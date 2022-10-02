# On-Off-Temperature-Control

In this project a tm4c123gh6pm MCU was used to monitor the temperatures read by a temperature sensor using the ADC module

The tempertaure setpoint was passed to the MCU using UART and the current temperature and state were displayed on the LCD.
When a critical temperature was surpassed a buzzer was turned on and a warning was given on the LCD. LEDs were also used to indicate state
There were 3 states
1. below setpoint: buzzer off blue LED on
2. between setpoint and alarm setpoint: buzzer off green LED on
3. above alarm setpoint: buzzer on  red LED on

The system was implemented using freeRTOS. There were 4 tasks:

1. InitTask: This task was used to initiailize all the hardware modules used (ADC,LCD,GPIOs,UART)
2. Setpoint_Task: used to get the setpoint from PC using UART
3. Temp_Measure_Task: triggered the ADC conversion to read the temperature sensor
4. Display_Task: displayed relevant info on the LCD

Queues were used for inter-task communication and task synchronization (through blocking).
