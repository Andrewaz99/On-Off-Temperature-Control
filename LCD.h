#ifndef LCD_H_
#define LCD_H_

#define LCDPORT         GPIO_PORTB_BASE
#define LCDPORTENABLE   SYSCTL_PERIPH_GPIOB
#define RS              GPIO_PIN_0
#define E               GPIO_PIN_1
#define D4              GPIO_PIN_4
#define D5              GPIO_PIN_5
#define D6              GPIO_PIN_6
#define D7              GPIO_PIN_7

void LCD_init(void);                    //LCD initialization
void LCD_Command(unsigned char c);      //Send command
void LCD_Print(char i, char *s,char j);	//Print specific row and column

#endif /* LCD_H_ */