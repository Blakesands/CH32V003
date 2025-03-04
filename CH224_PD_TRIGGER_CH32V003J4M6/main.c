/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : Green Dragon
 * Version            : V1.0.0
 * Date               : 11/2024
 * Description        : PD_Trigger PCB Driver
 * https://radicalbakers.org/green-dragon-maker-space
 ********************************************************************************/
/**
 * Parts used:  CH32V003 J4M6 RISC-V MCU 8 pin SO
 *              CH224K USB PD Protocol Sink Controller 10 pin essop
 *              NCP117-3.3 LDO Voltage Regulator
 *              WS2812B Addressable LED
 *              Button with 1k/1u RC debounce circuit
 **/

/* Include core and system files */
#include "debug.h"

// Addressable LED Driver
#include "PD_TRIGGER_WS2812_DRIVER.h"

// PD_Trigger PCB driver
#include "PD_TRIGGER_DRIVER.h"

int main (void){
    Delay_Init();
    LED_INIT();         // configure pin D6 as LED data output
    POWERGOOD_INIT();   // configure pin D1 as an input from CH224 Power Good signal (active low)
    PD_CONFIG_INIT();   // configure pins C1 C2 and C4 as PP outputs attached to cfg1 cfg2 cfg3 on the CH224
    REQ_5V();           // set cfg pins to request 5V
    EXTI0_INT_INIT();   // enable external hardware interrupt from button on A2

    while(1){
        __WFI();        // sleep mode waits for button press interrupt that increments V_REQ
        LED_SendColour(0, 0, 0, 100); // resets LED
        switch (V_REQ) { // requests voltages from source according to V_REQ
            case 0:
                REQ_5V();
                break;
            case 1:
                REQ_9V();
                break;
            case 2:
                REQ_12V();
                break;
            case 3:
                REQ_15V();
                break;
            case 4:
                REQ_20V();
                break;
            default:
                REQ_5V();
                break;
        }           // end switch
    }               // end while
}                   // end main
