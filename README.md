# Code for the RISC-V WCH CH32V003
(20-pins=20p 8-pins=10p in 2023)
This code drives WS2812B LEDs, Digipots, MOSFETs etc
https://radicalbakers.org/green-dragon-maker-space

# MCP4131 Digipot SPI

This is a driver for the MCP4131 Microchip Digital Potentiometer, written in C language for a CH32V003 microcontroller. The code is broken down into several functions that are called from the main function.

The first function, SPI_1Lines_HalfDuplex_Init(), initialises the SPI communication protocol for single-wire half duplex mode. It sets up the SPI peripheral pins. It sets up the SPI peripheral in 1-line transmission mode with the microcontroller acting as the master. The SPI clock polarity is set to high and the first bit is transmitted most significant bit first. Finally, the SPI is enabled.

The cs_init() function initializes pin C4 on the microcontroller, which is connected to the chip select pin of the MCP4131.

The incrementZ() and decrementZ() functions increment and decrement the wiper position of the MCP4131 digital potentiometer, respectively. They first put the chip select pin low to select the MCP4131. They then send the appropriate command over SPI to increment or decrement the wiper position of the MCP4131 by one step. After a delay of 9 microseconds to allow for the data transfer to complete, the chip select pin is set high to deselect the MCP4131.

In the main function, the initialisation functions are called and the MCP4131 is repeatedly incremented and decremented in a loop for testing porpoises.

# MCP4012 Digipot Bit-bang

This is a driver for the MCP4131 Microchip Digital Potentiometer, written in C language for a CH32V003 microcontroller. It uses direct port access (?) to toggle the pins and assembly language to insert NOP (No Operation) instructions, which are instructions that take up a clock cycle but do nothing, to control the precise timing of the bit-banging operations. It does the same thing as the code above...

# WS2812B Addressable LEDs Bit-bang

This is a driver for WS2812B Addressable LED strips, written in C language for a CH32V003 microcontroller. It uses direct port access (?) to toggle the pins and assembly language to insert NOP (No Operation) instructions, which are instructions that take up a clock cycle but do nothing, to control the precise timing of the bit-banging operations. This is V2 - V3 will use the CH32V003s PWM pulse function instead of bitbanging

It includes various effects I have coded pretty much from scratch.

# PWM_MOSFET

Drives a mosfet for 12v led PWM dimming. Button input, overcurrent monitor on IRq
