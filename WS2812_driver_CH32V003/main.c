// SWIO programming
#include "debug.h"
//#include "math.h"

// Define the number of LEDs in the strip and overall brightness
#define num_leds 3
#define BRIGHT 255

// Include the Green Dragon Bitbanging driver
#include "GD_WS2812_DRIVER.h"

// Include the sine and rand lookup tables
#include "lookups.h"

    // Include the colour and animation functions
#include "LED_functions.h"

    // Initialise the Delay and GPIO, set the initial colour values of the LED strip to off
int main(void){
    // Initialise delay
    Delay_Init();

    // configure GPIOC pin 4 as Output Push/Pull for data out
    DATA_INIT();
    Delay_Ms(10);
    LED_OFF();
    // configure GPIOC pin 1 as Input Pull Up for TP4056 charging indicator
//    INPUT_INIT();
//    Delay_Ms(10);

    // configure GPIOA pin 1 as Analogue input for seeding srand and seed srand
//    RANDSEED_INIT();
    Delay_Ms(250);

    // Send initial LED colour values to the LED strip


  // Loop
while (1) {
//    int nocharge = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1);
//    if(nocharge==1){
//            LED_COLD_WHITE();
//            LED_WARM_WHITE();
        LED_RAINBOWS(250,144); // variables are delay speed in mS and width (number of leds that cover one cycle)
//            LED_RAINBOWS_WIDTH(50);
//            LED_BLUEFIRE(35); // variable is speed in mS delay
//            LED_STATIC_RANDOM();
//            LED_RED(25); // variable is brightness 0-255
//            LED_GREEN(10); // variable is brightness 0-255
//            LED_BLUE(10); // variable is brightness 0-255
//            LED_CUSTOM(0,25,0); // set custom colour (red, green, blue) 0-255 RGB
//            LED_RGB_FLASH(333, 100); // variables are speed in mS delay and brightness
//            LED_WHITE_FLASH(50, 10); // variables are max and min flash speed
//            LED_WHITE_FADE(4); // variable is speed in mS delay
//            LED_PD_TRIGGER (1, 1000);
//        } // end of if

    } // end of while
} // end of Main

