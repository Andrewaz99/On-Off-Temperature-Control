# On-Off-Temperature-Control

In this project a tm4c123gh6pm MCU was used to monitor the temperatures read by a temperature sensor using the ADC module

The tempertaure setpoint was passed to the MCU using UART and the current temperature and state were displayed on the LCD.
When a critical temperature was surpassed a buzzer was turned on and a warning was given on the LCD. LEDs were also used to indicate state
There were 3 states
1. below setpoint: buzzer off blue LED on
2. between setpoint and alarm setpoint: buzzer off green LED on
3. above alarm setpoint: buzzer on  red LED on
