# Code for the RISC-V WCH CH32V003 by https://radicalbakers.org/green-dragon-maker-space

This code drives WS2812B LEDs, Digipots, MOSFETs etc.. It's been a learning curve. I have been coding for Arduino at a very basic level since 2019. I require basic functions from microcontrollers. I am from Yorkshire. Enter the 10p microcontroller... (20-pins=20p 8-pins=10p in 2023)

The trade-off for muchos cheapness is menos libraries. Here are a few code snippets i hope will help other hobbyists get going with these V.Cheap MCUs.
The example code and manuals are really good and can be found at https://github.com/openwch/ch32v003

These files were developed in WCH's IDE Mounriver Studio (Mooóóoon River!). You can rename them main.c and copy them over the main.c in your project user folder.
You can buy the chips and the programmer from the WCH outlet on Aliexpress https://www.aliexpress.com/item/1005005036714708.html? (no im not a shill)

#TODO notes on flashing the chips esp J4M6 8-pin

# MCP4131 Digipot SPI

This is a very basic SPI driver for the MCP4131 Microchip Digital Potentiometer, written in C language for a CH32V003 microcontroller. It can only increment and decrement the wiper without any of the error checking and status reading functions. It can be easily expanded to 2 channel versions of the ICs.

>SPI_1Lines_HalfDuplex_Init() initialises the SPI communication protocol for single-wire half duplex mode. It sets up the SPI peripheral pins. It sets up the SPI peripheral in 1-line transmission mode with the microcontroller acting as the master (Host). The SPI clock polarity is set to high and the first bit is transmitted most significant bit first. Finally, the SPI is enabled.

>The cs_init() function initialises pin C4 on the microcontroller, which is connected to the chip select pin of the MCP4131.

>The incrementZ() and decrementZ() functions increment and decrement the wiper position of the MCP4131 digital potentiometer, respectively. They first put the chip select pin low to select the MCP4131. They then send the appropriate 8-bit command and 8 clocks over the SPI pins to increment or decrement the wiper position of the MCP4131 by one step. After a delay of 9 microseconds to allow for the data transfer to complete, the chip select pin is set high to deselect the MCP4131.

In the main function, the initialisation functions are called and the MCP4131 is repeatedly incremented and decremented in a loop for testing porpoises.

# MCP4012 Digipot Bit-bang

This is a driver for the MCP4131 Microchip Digital Potentiometer, written in C language for a CH32V003 microcontroller. It uses direct port access (?) to toggle the pins and assembly language to insert NOP (No Operation) instructions, which are instructions that take up a clock cycle but do nothing, to control the precise timing of the bit-banging operations. It does a similar thing to the code above but less gracefully...

# WS2812B Addressable LEDs Bit-bang

This is a driver for WS2812B Addressable LED strips, written in C language for a CH32V003 microcontroller. It uses direct port access (i think its called this?) to toggle the pins and assembly language to insert NOP (No Operation) instructions, which are instructions that take up a clock cycle but do nothing, to control the precise timing of the bit-banging operations. This is V2 - V3 will use the CH32V003s PWM pulse function instead of bitbanging. It ain't FASTLED but it works...

It includes various effects I have coded pretty much from scratch. Much more fun than all this Arduino Library nonsense... :D :D :D

# PWM_MOSFET

Drives a mosfet for 12v led PWM dimming. Button input, overcurrent monitor on IRq



