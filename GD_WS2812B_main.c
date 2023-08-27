// For SWIO programming
#include "debug.h"

// Define the number of LEDs in the strip
#define num_leds 2

// Include the sine and rand lookup tables
#include "lookups.h"

// Include the Green Dragon Bitbanging driver
#include "GD_WS2812_DRIVER.h"

// Include the colour and animation functios
#include "LED_functions.h"



// The main function initialises the Delay and GPIO, sets the initial color values of the LED strip to off
int main(void)
    {
    // Initialise delay
    Delay_Init();
    // configures GPIOC pin 4 as Output Push/Pull for data out
    GPIO_INITZ();
    Delay_Ms(5);
    // Send initial LED colour values to the LED strip
    LED_OFF();
    SHOWTIME(BUFFER_LEDS);
    Delay_Ms(200);

  // Loop
    while (1) {

//            LED_COLD_WHITE();
//            LED_WARM_WHITE();
            LED_RAINBOWS(100,2); // variables are delay speed in mS and width (number of leds)
//            LED_BLUEFIRE(30); // variable is speed in mS delay
//            LED_STATIC_RANDOM();
//            LED_RED(255); // variable is brightness 0-255
//            LED_GREEN(10); // variable is brightness 0-255
//            LED_BLUE(10); // variable is brightness 0-255
//            LED_CUSTOM(255,255,255); // set custom clour (red, green, blue) 0-255 RGB
//            LED_RGB_FLASH(200, 2); // variables are speed in mS delay and brightness
//            LED_WHITE_FLASH(50, 10); // variables are max and min flash speed
//            LED_WHITE_FADE(4); // variable is speed in mS delay


    } // end of while
} // end of Main
