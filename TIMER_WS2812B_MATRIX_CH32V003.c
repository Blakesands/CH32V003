/* This code drives a strip of 140 WS2812B LEDs 5*7 x 4 digits display.
 * It uses no external libraries.
 * It counts up in seconds to 99m59s and resets.
 * The code is written in C for a CH32V003 WCH Microcontroller.

The code includes two arrays, Digit_Map and DigitS.
Digit_Map defines the layout of the LED display for each digit from 0-9.
DigitS stores the digit of the time or count number that will be displayed .

The SysTick_Handler function is called by NVIC timer interrupt SysTick_Irq
every millisecond. The SysTick_Handler increments the COUNT variable,
which is used by the display_number function to update the LED display.
The CAL variable is done by hand for each install to 1s over 99 minutes.

The main function initializes the LED GPIO and delay and loops forever
animating the LED strip. The loop calls the display_number function which
converts the count variable into an array of digits, loops through each digit
and turns on the appropriate LEDs in the LED strip according to the Digit_Map.
 */

#include "debug.h"  // For debugging and printing

// Define a variable to store the raw count
float COUNT = 0.0;

// Calibration to <1s at 99 mins
float CAL = 19.42795; // /103.4

// Define an array to hold the digit info
int DigitS[4];

// Define the mapping of LEDs to digits
int Digit_Map[10][35] = {

        {
        0, 1, 1, 1, 0,
        1, 0, 0, 0, 1,
        1, 0, 0, 0, 1,
        1, 0, 0, 0, 1,
        1, 0, 0, 0, 1,
        1, 0, 0, 0, 1,
        0, 1, 1, 1, 0
        }, // 0

        {
        0, 0, 1, 0, 0,
        0, 1, 1, 0, 0,
        1, 0, 1, 0, 0,
        0, 0, 1, 0, 0,
        0, 0, 1, 0, 0,
        0, 0, 1, 0, 0,
        1, 1, 1, 1, 1
        }, // 1

        {
        1, 1, 1, 1, 0,
        0, 0, 0, 0, 1,
        0, 0, 0, 0, 1,
        0, 1, 1, 1, 0,
        1, 0, 0, 0, 0,
        1, 0, 0, 0, 0,
        1, 1, 1, 1, 1
        }, // 2

        {
        1, 1, 1, 1, 0,
        0, 0, 0, 0, 1,
        0, 0, 0, 0, 1,
        0, 1, 1, 1, 0,
        0, 0, 0, 0, 1,
        0, 0, 0, 0, 1,
        1, 1, 1, 1, 0
        }, // 3

        {
        0, 0, 0, 1, 0,
        0, 0, 1, 1, 0,
        0, 1, 0, 1, 0,
        1, 0, 0, 1, 0,
        1, 1, 1, 1, 1,
        0, 0, 0, 1, 0,
        0, 0, 0, 1, 0
        }, // 4

        {
        1, 1, 1, 1, 1,
        1, 0, 0, 0, 0,
        1, 0, 0, 0, 0,
        0, 1, 1, 1, 0,
        0, 0, 0, 0, 1,
        0, 0, 0, 0, 1,
        1, 1, 1, 1, 0
        }, // 5

        {
        0, 1, 1, 1, 1,
        1, 0, 0, 0, 0,
        1, 0, 0, 0, 0,
        1, 1, 1, 1, 0,
        1, 0, 0, 0, 1,
        1, 0, 0, 0, 1,
        0, 1, 1, 1, 0
        }, // 6

        {
        1, 1, 1, 1, 1,
        0, 0, 0, 0, 1,
        0, 0, 0, 1, 0,
        0, 0, 1, 0, 0,
        0, 1, 0, 0, 0,
        1, 0, 0, 0, 0,
        1, 0, 0, 0, 0
        }, // 7

        {
        0, 1, 1, 1, 0,
        1, 0, 0, 0, 1,
        1, 0, 0, 0, 1,
        0, 1, 1, 1, 0,
        1, 0, 0, 0, 1,
        1, 0, 0, 0, 1,
        0, 1, 1, 1, 0
        }, // 8

        {
        0, 1, 1, 1, 0,
        1, 0, 0, 0, 1,
        1, 0, 0, 0, 1,
        0, 1, 1, 1, 1,
        0, 0, 0, 0, 1,
        0, 0, 0, 0, 1,
        0, 0, 0, 0, 1
        }, // 9
  };

// Initialize the GPIO wiggle pin
void GPIO_INITZ(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

// Send a single bit using the GD pinwiggle protocol.
void LED_SendBit(uint8_t bit)
{
    if (bit) {
        // For a logical 1, send high-high-high-low signal to the GPIO pin
        GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_SET);
        GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_SET);
        GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_SET);
        GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_RESET);
    } else {
        // For a logical 0, send high-low-low-low signal to the GPIO pin
        GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_SET);
        GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_RESET);
        GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_RESET);
        GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_RESET);
    }
}

// Send a single colour for a single LED
void LED_SendColour(uint8_t red, uint8_t green, uint8_t blue)
{
    // Send the green component first (MSB)
    for (int i = 7; i >= 0; i--) {
        LED_SendBit((green >> i) & 1);
    }
    // Send the red component next
    for (int i = 7; i >= 0; i--) {
        LED_SendBit((red >> i) & 1);
    }
    // Send the blue component last (LSB)
    for (int i = 7; i >= 0; i--) {
        LED_SendBit((blue >> i) & 1);
    }
}

void display_number(int number) {
  // Convert the number to an array of digits
  DigitS[0] = number / 1000;
  DigitS[1] = (number / 100) % 10;
  DigitS[2] = (number / 10) % 10;
  DigitS[3] = number % 10;

  // Turn on the appropriate LEDs for each digit
  for (int i = 0; i < 2; i++) { // loop through digits
    for (int j = 0; j < 35; j++) { // loop through 35 leds and read digit map
      if (Digit_Map[DigitS[i]][j] == 1) {
        LED_SendColour(50, 0, 0);
      } //
      else {
          LED_SendColour(0, 0, 0);
      } // end of else

//      Delay_Us(50); // delay to let WS2812B LEDs know to cycle
    }
  }
  for (int i = 2; i < 4; i++) { // loop through digits
    for (int j = 0; j < 35; j++) { // loop through 35 leds and read digit map
      if (Digit_Map[DigitS[i]][j] == 1) {
        LED_SendColour(0, 0, 50);
      } //
      else {
          LED_SendColour(0, 0, 0);
      } // end of else

//      Delay_Us(50); // delay to let WS2812B LEDs know to cycle
    }
  }
}

void display_time(int number) {
  // Convert the number to an array of 4 digit clock display
  DigitS[0] = (number / 600) % 10;
  DigitS[1] = (number / 60) % 10;
  DigitS[2] = (number / 10) % 6;
  DigitS[3] = number % 10;

  // Turn on the appropriate LEDs for each digit
  for (int i = 0; i < 2; i++) { // first 2 digits are red
    for (int j = 0; j < 35; j++) { // loop through 35 leds and read digit map
      if (Digit_Map[DigitS[i]][j] == 1) {
        LED_SendColour( 10, 0, 0);
      } //
      else {
          LED_SendColour(0, 0, 0);
      } // end of else
    }
  }
  for (int i = 2; i < 4; i++) { // second 2 digits are pink
    for (int j = 0; j < 35; j++) { // loop through 35 leds and read digit map
      if (Digit_Map[DigitS[i]][j] == 1) {
        LED_SendColour(10, 0, 100);
      } //
      else {
          LED_SendColour(0, 0, 0);
      } // end of else

//      Delay_Us(50); // delay to let WS2812B LEDs know to cycle
    }
  }
}

void SysTick_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

void SysTick_Handler(void)
{
    COUNT++;
    SysTick->SR &= ~(1 << 0);
}

// The main function initialises the GPIO and Delay, sets the initial color values
// of the LED strip, and loops forever animating the LED strip.
int main(void)
{
    Delay_Init(); // Initialise delay,
    GPIO_INITZ(); // configures pin C4 as Output Push/Pull for data out
    display_time(0);
    Delay_Ms(10);
    SysTick->SR &= ~(1 << 0); // flag for systick
//    SysTick->CMP = SystemCoreClock;
//    SysTick->CNT = 0;
    SysTick->CTLR = 0xf; // control register for systick
    NVIC_EnableIRQ(SysTicK_IRQn);
//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

// Main loop
while (1) {
//    if (COUNT>(5999*CAL)) {
//        COUNT=0;
//    }
    display_time(COUNT/CAL); // calibrated to <1sec @ 10mins
    Delay_Ms(333);
//    PWR_EnterSTANDBYMode(PWR_STANDBYEntry_WFE);
    } // end of while
} // end of Main
