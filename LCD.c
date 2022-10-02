#include <stdbool.h>
#include <stdint.h>
#include "lcd.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "string.h"

void LCD_init()
{
	SysCtlPeripheralEnable(LCDPORTENABLE); 	//Connecting clock to LCD port (Port B)
	GPIOPinTypeGPIOOutput(LCDPORT, 0xFF); 	//Set LCD port as output.
	SysCtlDelay(50000);
	GPIOPinWrite(LCDPORT, RS,  0x00 );
	GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7,  0x30 );
	GPIOPinWrite(LCDPORT, E, 0x02);
	SysCtlDelay(10);
	GPIOPinWrite(LCDPORT, E, 0x00);
	SysCtlDelay(50000);
	GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7,  0x30 );
	GPIOPinWrite(LCDPORT, E, 0x02);
	SysCtlDelay(10);
	GPIOPinWrite(LCDPORT, E, 0x00);
	SysCtlDelay(50000);
	GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7,  0x30 );
	GPIOPinWrite(LCDPORT, E, 0x02);
	SysCtlDelay(10);
	GPIOPinWrite(LCDPORT, E, 0x00);
	SysCtlDelay(50000);
	GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7,  0x20 );
	GPIOPinWrite(LCDPORT, E, 0x02);
	SysCtlDelay(10);
	GPIOPinWrite(LCDPORT, E, 0x00);
	SysCtlDelay(50000);
	
	LCD_Command(0x0F); 	//Turn on Lcd
	LCD_Command(0x01);	//Clear screen
	SysCtlDelay(160000);

}

void LCD_Command(unsigned char c)
{
	GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (c & 0xf0) );
  GPIOPinWrite(LCDPORT, RS, 0x00);
  GPIOPinWrite(LCDPORT, E, 0x02);
	SysCtlDelay(50000);
	GPIOPinWrite(LCDPORT, E, 0x00);
	SysCtlDelay(50000);
	GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (c & 0x0f) << 4 );
	GPIOPinWrite(LCDPORT, RS, 0x00);
	GPIOPinWrite(LCDPORT, E, 0x02);
	SysCtlDelay(10);
	GPIOPinWrite(LCDPORT, E, 0x00);
	SysCtlDelay(50000);
}

void LCD_Show(unsigned char d)
{
	GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (d & 0xf0) );
	GPIOPinWrite(LCDPORT, RS, 0x01);
	GPIOPinWrite(LCDPORT, E, 0x02);
	SysCtlDelay(10);
	GPIOPinWrite(LCDPORT, E, 0x00);
	SysCtlDelay(50000);
	GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (d & 0x0f) << 4 );
	GPIOPinWrite(LCDPORT, RS, 0x01);
	GPIOPinWrite(LCDPORT, E, 0x02);
	SysCtlDelay(10);
	GPIOPinWrite(LCDPORT, E, 0x00);
	SysCtlDelay(50000);
}

void LCD_Cursor(char x, char y)
{
	if (x==0)
		{
			LCD_Command(0x80 + (y % 16));
      return;
    }
  LCD_Command(0xC0 + (y % 16));
}

void LCD_Print(char i, char *s,char j)
{
	LCD_Cursor(i, j);
	for (i=0; i<strlen(s); i++)
	{
		LCD_Show(s[i]);
	}
	LCD_Command(0xC0 + 16); //Hide cursor
}