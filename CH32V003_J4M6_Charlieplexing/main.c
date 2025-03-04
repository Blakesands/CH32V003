/* main.c Created on: 04/03/25 Author: BK Mounriver IDE
 * Charlieplexing 30 LEDs using 6 pins on a CH32V003J4M6 microcontroller.*/

// in system_ch32v00x.c
// #define SYSCLK_FREQ_48MHZ_HSI   48000000

// include core and system files
#include "debug.h"

// Define the number of LEDs and pins
#define NUM_LEDS 30
#define NUM_PINS 6

// Define the length of the animation and when to switch frames
#define ANIMATION heart // line arrow heart
#define NUMBER_FRAMES 50000 // 10k is approx 1 second, 50k for heart, 4k for line/arrow
#define NUMBER_ANIMATION_FRAMES 10
#define FRAME0_TIME 0
#define CUSTOM_TIMING
//#define AUTO_TIMING

#ifdef CUSTOM_TIMING
// Define custom frame timings (for heart in this case)
#define FRAME1_TIME 10000
#define FRAME2_TIME 11000
#define FRAME3_TIME 12000
#define FRAME4_TIME 13000
#define FRAME5_TIME 14000
#define FRAME6_TIME 15000
#define FRAME7_TIME 16000
#define FRAME8_TIME 18000
#define FRAME9_TIME 30000
#endif

#ifdef AUTO_TIMING
// Or set to auto frame rate (for line, arrow)
#define FRAME1_TIME (NUMBER_FRAMES/NUMBER_ANIMATION_FRAMES)
#define FRAME2_TIME FRAME1_TIME*2
#define FRAME3_TIME FRAME1_TIME*3
#define FRAME4_TIME FRAME1_TIME*4
#define FRAME5_TIME FRAME1_TIME*5
#define FRAME6_TIME FRAME1_TIME*6
#define FRAME7_TIME FRAME1_TIME*7
#define FRAME8_TIME FRAME1_TIME*8
#define FRAME9_TIME FRAME1_TIME*9
#endif

// Includes
#include <animations.h>
#include <charlieplexing.h>
#include <LED_functions.h> // some unused functions

int main(void){
    GPIO_Config();
    start_systick ();
    while (1){
        animate_frames ();
    }
}
