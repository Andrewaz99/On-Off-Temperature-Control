#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "MyInitiation.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "semphr.h"
#include "tm4c123gh6pm.h"
#include "lcd.h"

xQueueHandle xLCDQueue;
xQueueHandle xUARTQueue;

volatile int setpoint = 30;		//Default value.
volatile int AlarmValue = 80;	//Preset value.
volatile int temp_C;

void InitTask(void)
{
	LED_Init();
	ADC0_Init();
	UART0_Init();
	LCD_init();
	
	vTaskSuspend(NULL);
}
void Setpoint_Task(void)
{
	char c;
	char snum[5];
	int SendSetpoint;
	int i = 0;
	UART_Print("Enter the required setpoint in degree celsius: ");
	LCD_Print(0,"Enter Setpoint..",0);
	
	/*Receive Setpoint value from user using UART*/
	while(1)
	{
		while((UART0_FR_R&(1<<4))!=0);
		c = UART0_DR_R;
		snum[i] = c;
		if(c == '\r') break;
		i++;
	}
	
	UART_Print("\n\rSetpoint set to : ");
	UART_Print(snum);
	UART_Print("\n\r");
	LCD_Print(0,"Setpoint SET!!!!",0);
	SysCtlDelay((SysCtlClockGet() / 3)*2);	//Delay 2 seconds
	LCD_Command(0x01);											//Clear screen
	LCD_Print(0,"Setpoint: ",0);
	LCD_Print(0,snum,10); 									
	LCD_Print(0,"C",12);
	
	SendSetpoint = atoi(snum);							//Change string to int
	xQueueSendToBack(xUARTQueue,&SendSetpoint,1000/portTICK_PERIOD_MS);
}
void Temp_Measure_Task(void)
{
	xQueueReceive(xUARTQueue,&setpoint,portMAX_DELAY);
	while(1)
	{
		char snum[5];
		itoa(setpoint, snum, 10);									//Convert int to string
		ADC0_PSSI_R |= 8;           							//Start a conversion sequence 3
		while((ADC0_RIS_R & 0x08) == 0);					//Wait for conversion.                  
		temp_C = ((ADC0_SSFIFO3_R * 330) / 4096);	//Read conversion result and Convert it to celsius
		ADC0_ISC_R = 8;             							//Clear completion flag
		
		/*Checking the mesaured temprature to select the required action*/
		if(temp_C<setpoint)
		{
			Toggle_BLUE_LED();
		}
		else if(temp_C>setpoint && temp_C<AlarmValue)
		{
			LCD_Print(0,"Setpoint: ",0);
			LCD_Print(0,snum,10);
			LCD_Print(0,"C   ",12);
			Toggle_GREEN_LED();
		}
		else if(temp_C>AlarmValue)
		{
			LCD_Print(0,"ALARM! (TEMP>80)",0);
			Toggle_RED_LED();
		}
		
		xQueueSendToBack(xLCDQueue,&temp_C,1000/portTICK_PERIOD_MS);
	}
}
void Display_Task(void)
{
	int ReceviedTemp;
	char snum[5];
	portBASE_TYPE xStatus;
	while(1)
	{
		xStatus = xQueueReceive(xLCDQueue,&ReceviedTemp,portMAX_DELAY);
		if(xStatus == pdPASS)
		{
			itoa(ReceviedTemp, snum, 10);	//Convert int to string
			LCD_Print(1,"Measured: ",0);
			LCD_Print(1,snum,10); 				//start of heading,string,"/n"
			LCD_Print(1,"C",12);				// start of heading,string,"/f"
		}
	}
}

int main()
{
	xLCDQueue = xQueueCreate(5,sizeof(int));
	xUARTQueue = xQueueCreate(5,sizeof(int));
	
	if(xLCDQueue != NULL)
	{
		xTaskCreate(InitTask,		 			"Init ADC0, LCD, LEDs, UART0",100,NULL,4,NULL);
		xTaskCreate(Display_Task,			"Display temp using LCD",			100,NULL,3,NULL);
		xTaskCreate(Temp_Measure_Task,"Measure temp using ADC",			100,NULL,2,NULL);
		xTaskCreate(Setpoint_Task,		"Set the setpoint using UART",100,NULL,1,NULL);
		vTaskStartScheduler();
	}
	else
	{}
	while (1);
	return 0;
}