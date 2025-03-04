# Code for the RISC-V WCH CH32V003 by https://radicalbakers.org/green-dragon-maker-space

This code drives WS2812B LEDs, Digipots, MOSFETs etc.. It's been a learning curve. I have been coding for Arduino at a very basic level since 2019. I require basic functions from microcontrollers. I am from Yorkshire. Enter the 10p microcontroller... (20-pins=20p 8-pins=10p in 2023)

The trade-off for muchos cheapness is menos libraries. Here are a few code snippets i hope will help other hobbyists get going with these V.Cheap MCUs.
The example code and manuals are really good and can be found at https://github.com/openwch/ch32v003

These files were developed in WCH's IDE Mounriver Studio (Mooóóoon River!). You can rename the .c file main.c and copy the files into your project folder, 
You can buy the chips and the programmer from the WCH outlet on Aliexpress https://www.aliexpress.com/item/1005005036714708.html? (no im not a shill)

The 8 pin version of this CH32V003 MCU is the J4M6. To program it you need the WCH Link-E programmer. It needs only 3 lines connecting. + voltage, - voltage and SWIO that goes to pin 8 on the IC

# Charlieplexing 30 LEDs with 6 pins on the CH32V003 J4M6

LEDs are controlled by setting pins as high, low, or high-impedance.
bool led_buffer holds the state of each LED. Only 1 LED is lit at a time.
Systick is used to update the LEDs at approx 10kHz

![charlieplexing schematic](https://github.com/Blakesands/CH32V003/blob/5f146b100ca555e7c42f938b3817a3958abd47fd/charlieplexing_j4m6/charlieplexing_ch32v003.jpg)

# WS2812B Addressable LEDs Bit-bang

This is a driver for WS2812B Addressable LED strips, written in C language for a CH32V003 microcontroller. It uses direct port access to toggle the pins and assembly language to insert NOP (No Operation) instructions, which are instructions that take up a clock cycle but do nothing, to control the precise timing of the bit-banging operations. This is V2 - V3 will use the CH32V003s PWM pulse function instead of bitbanging. It ain't FASTLED but it works...

It includes various effects I have coded, some from scratch. Much more fun than all this Arduino Library nonsense... :D :D :D

# MCP4131 Digipot SPI

This is a very basic SPI driver for the MCP4131 Microchip Digital Potentiometer, written in C language for a CH32V003 microcontroller. It can only increment and decrement the wiper without any of the error checking and status reading functions. It can be easily expanded to 2 channel versions of the ICs.

>SPI_1Lines_HalfDuplex_Init() initialises the SPI communication protocol for single-wire half duplex mode. It sets up the SPI peripheral pins. It sets up the SPI peripheral in 1-line transmission mode with the microcontroller acting as the master (Host). The SPI clock polarity is set to high and the first bit is transmitted most significant bit first. Finally, the SPI is enabled.

>The cs_init() function initialises pin C4 on the microcontroller, which is connected to the chip select pin of the MCP4131.

>The incrementZ() and decrementZ() functions increment and decrement the wiper position of the MCP4131 digital potentiometer, respectively. They first put the chip select pin low to select the MCP4131. They then send the appropriate 8-bit command and 8 clocks over the SPI pins to increment or decrement the wiper position of the MCP4131 by one step. After a delay of 9 microseconds to allow for the data transfer to complete, the chip select pin is set high to deselect the MCP4131.

In the main function, the initialisation functions are called and the MCP4131 is repeatedly incremented and decremented in a loop for testing porpoises.

# MCP4012 Digipot Bit-bang

This is a driver for the MCP4012 Microchip Digital Potentiometer, written in C language for a CH32V003 microcontroller. It uses direct port access (?) to toggle the pins and assembly language to insert NOP (No Operation) instructions, which are instructions that take up a clock cycle but do nothing, to control the precise timing of the bit-banging operations. It does a similar thing to the code above but less gracefully...

# PWM_MOSFET

This code is for the Green Dragon CH32V003F4P6 Microcontroller Breakout Board, it implements a variable duty cycle PWM LED MOSFET dimmer with buttons and current sensing. The system can monitor the current across the MOSFET and pull the MOS gate low to shut off the PWM signal
if an over-current condition is detected. It will resume with previous settings if the over-current condition is removed.

Function Descriptions:

>The KEY_Init function initialises two buttons (KEY0 and KEY1) by configuring GPIO pins C1 and C2 as input with
internal pull-down resistors. The KEY_PRESS and KEY_PRESS_1 functions read the state of the buttons, de-bounce them
and return 1 if a button is pressed, 0 otherwise.

>The TIM1_PWMOut function initialises Timer 1 and outputs a 19.8kHz variable duty cycle PWM signal on GPIO pin D2.

>The OPA1_Init function initialises an operational amplifier (OPA1) by configuring GPIO pins D7 as the OP+ and
D0 as OP- inputs and then enabling the OPA with output on D4 which is the ADC input channel 7. The OPA Inverting input must be
externally connected via a resistor to OPA Output and connected to ground via a resistor to create a non-inverting amplifier
for current sensing across the MOSFET.

>The ADC_Channel7_Init function initialises analogue-to-digital converter (ADC) channel 7. The Get_ADC_Val and Get_ADC_Average
functions read the ADC value and calculate the average of multiple readings.

>The Monitor function is called periodically from the SysTicK_IRQ interrupt handler and reads the current sensing ADC value
to check for an over-current condition. If an over-current condition is detected, the PWM signal is disabled and a Warning Overcurrent
message is printed to the console. If the current returns within the acceptable range, the PWM signal is enabled, and a status: Good
message is printed.

>Main(void) calls the functions that initialise various parts of the system, including GPIO pins, timers and interrupts.

>While(1) continuously loops and, if the OC flag is not set, checks for key presses. It then adjusts the PWM duty cycle and therefore
the LED brightness based on the button pressed.

# TIMER_MATRIX_LEDS

This code drives a strip of 140 WS2812B LEDs with 4 digits, each digit being 7 rows * 5 columns with the rows in each digit starting at the left and top. It uses no external libraries. It initialises a Timer that counts up in seconds to 99m59s and resets. The code is written in C for a CH32V003 WCH Microcontroller. The CAL variable is done by hand and will poobrably need adjusting for each MCU.

>It includes two arrays, Digit_Map and DigitS. Digit_Map defines the layout of the LED display for each digit from 0-9. DigitS stores the digit of the count number that will be displayed.

>The SysTick_Handler function is called by NVIC timer interrupt SysTick_Irq every millisecond. The SysTick_Handler increments the COUNT variable, which is used by the display_number function to update the LED display.

>The main function initializes the LED GPIO and delay and loops forever animating the LED strip. The loop calls the display_number function which converts the count variable into an array of digits, loops through each digit and turns on the appropriate LEDs in the LED strip according to the Digit_Map.
