# Code for the RISC-V WCH CH32V003 by https://radicalbakers.org/green-dragon-maker-space

This code drives WS2812B LEDs, Digipots, MOSFETs etc.. It's been a learning curve. I have been coding for Arduino at a very basic level since 2019. I require basic functions from microcontrollers. I am from Yorkshire. Enter the 10p microcontroller... (20-pins=20p 8-pins=10p in 2023). The trade-off for muchos cheapness is menos libraries. Here are a few code snippets i hope will help other hobbyists get going with these V.Cheap MCUs.

These files were developed in WCH's IDE Mounriver Studio (Mooóóoon River!). 
The example code and manuals can be found at https://github.com/openwch/ch32v003
Also there is CNLohr's alternative to the WCH HAL. https://github.com/cnlohr/ch32fun
And these chips can now be programmed with the Arduino IDE! 

The 8 pin version of this CH32V003 MCU is the J4M6. To program it you need the WCH Link-E programmer. It needs only 3 lines connecting. + voltage, - voltage and SWIO that goes to pin 8 on the IC. You can buy the chips and the programmer from the WCH outlet on Aliexpress.

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

# Soft Latching NMOS Switch driver

CH32V003 J4M6 Soft latching switch with power off communication to main system
pins:
1. PA1       Nmos gate drive
2.           Ground 0V
3.           NC
4.           VDD +5V
5. PC1       Indicator LED
6. PC2       Button Press sense
7. PC4       AUX output, signal back to host
8. PD4       SWIO programming
  
![NMOS Switch Shematic](CH32V003J4M6_Soft_Latch_Switch/Soft_Latching_Switch_CH32V003.jpg)
