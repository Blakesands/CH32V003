/* main.c Created on: 04/03/25 Author: BK with WCH Mounriver IDE
 * Charlieplexing 30 LEDs using 6 pins on a CH32V003J4M6 microcontroller.*/

#include "charlieplexing.h"

int main(void){
    GPIO_Config();
    start_systick ();
    while (1){
        animate_frames ();
    }
}
