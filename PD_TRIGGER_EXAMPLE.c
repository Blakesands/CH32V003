/********************************** (C) COPYRIGHT *******************************
 * File Name          : PD_TRIGGER_EXAMPLE.c
 * Author             : Green Dragon
 * Version            : V1.0.0
 * Date               : 11/2024
 * Description        : PD_Trigger PCB Driver
 * https://radicalbakers.org/green-dragon-maker-space
 ********************************************************************************/
/**
 * Parts used:  CH32V003 J4M6 RISC-V MCU
 *              CH224K USB PD Protocol Sink Controller
 *              NCP117-3.3 LDO Voltage Regulator
 *              WS2812B Addressable LED
 *              Button with 1k/1u RC debounce circuit
 **/

// #define SYSCLK_FREQ_48MHZ_HSI   48000000 // in system_ch32v00x.c

/* Includes */
#include "debug.h"

// Addressable LED Driver
#include "PD_TRIGGER_WS2812_DRIVER.h"

// Green Dragon PD_Trigger PCB driver
#include "PD_TRIGGER_DRIVER.h"

int main (void)
{
    Delay_Init();
    LED_INIT(); // sets pin D6 as LED data output
    POWERGOOD_INIT(); // sets pin D1 as an input from CH224 Power Good signal (active low)
    PD_CONFIG_INIT(); // sets pins C1 C2 and C4 as PP outputs attached to cfg1 cfg2 cfg3 on the CH224
    REQ_5V();
    EXTI0_INT_INIT(); // external hardware interrupt from button on A2

    while(1)
    {
        __WFI(); // sleep mode waits for button press interrupt
        LED_SendColour(0, 0, 0, 1); // interrupt increments V_REQ and resets LED
        switch (V_REQ) // requests voltages from source according to V_REQ
        {
        case FIVE:
            REQ_5V();
            break;
        case NINE:
            REQ_9V();
            break;
        case TWELVE:
            REQ_12V();
            break;
        case FIFTEEN:
            REQ_15V();
            break;
        case TWENTY:
            REQ_20V();
            break;
        default:
            REQ_5V();
            break;
        } // end switch
    } // end while
} // end main
