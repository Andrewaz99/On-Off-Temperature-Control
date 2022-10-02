#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include "driverlib/gpio.h"
#include <driverlib/sysctl.h>
#include "tm4c123gh6pm.h"
#include "driverlib/adc.c"
#include "driverlib/uart.c"
#include "MyInitiation.h"

void LED_Init()
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);             											//Enable peripheral clock for GPIOF.
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));    												//Wait for GPIOF to be enabled.
  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, (GPIO_PIN_1 |GPIO_PIN_2|GPIO_PIN_3));	//Setting PF1,PF2,PF3 as outputs.
}
void Toggle_BLUE_LED()
{
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0); 					//Red.
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);	//Blue.
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0); 					//Green.
}
void Toggle_GREEN_LED()
{
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);						//Red.
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);						//Blue.
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);	//Green.
}
void Toggle_RED_LED()
{
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);	//Red.
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);						//Blue.
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);						//Green.
}
void ADC0_Init()
{
	/*Enable clocks*/
	SYSCTL_RCGCGPIO_R |= 0x10;      //Enable peripheral clock for GPIOE.
	SYSCTL_RCGCADC_R |= 1;          //Enable peripheral clock for ADC0.

	/*Initialize PE3 for AIN0 input*/
	GPIO_PORTE_AFSEL_R |= 8;        //Enable alternate function.
	GPIO_PORTE_DEN_R &= ~8;         //Disable digital function.
	GPIO_PORTE_AMSEL_R |= 8;        //Enable analog function.

	/*Initialize ADC0 */
	ADC0_ACTSS_R &= ~8;             //Disable SS3 during configuration.
	ADC0_EMUX_R &= ~0xF000;         //Software trigger conversion.
	ADC0_SSMUX3_R = 0;              //Get input from channel 0.
	ADC0_SSCTL3_R |= 6;             //Take one sample at a time, set flag at 1st sample.
	ADC0_ACTSS_R |= 8;              //Enable ADC0 sequencer 3.
}

void UART0_Init()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);                            //Enable peripheral clock for GPIOA.
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA)) {}                  //Wait for GPIOA to be enabled.
    GPIOPinConfigure(0x00000001);                                        		//Configure The UART Receiver Alternative Function.
    GPIOPinConfigure(0x00000401);                                        		//Configure The UART Transmitter Alternative Function.
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);              //Configure The Pins To UART.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);                            //Enable UART Clock.
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0)) {}                  //Wait for UART0 to be enabled..
    UARTDisable(UART0_BASE);                                                //Disable the UART.
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 9600,
    (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));    //Configure UART Baud Rate, Clock, and Parity Check.
    UARTEnable(UART0_BASE);                                                 //Enable UART
    UARTFIFODisable(UART0_BASE);                                            //Disable FIFOs
}

char* itoa(int value, char* result, int base) //Convert Integer to String
{
    //Check that the base if valid
    if (base < 2 || base > 36) { *result = '\0'; return result; }

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while(ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}

void UART_Print(char *string)      //Print String To UART terminal.
{
    while (*string)
    {
			UARTCharPut(UART0_BASE, *string);
			string++;
    }
}