/* Main.c Created on: 9 Mar 2025 Author: BK
 * CH32V003 J4M6 Soft latching switch with power off communication to main system */

#include <stdbool.h>
#include "Soft_Latch.h"

int main(void){

    SystemCoreClockUpdate();
    Delay_Init();
    Delay_Ms(10); // delay to prevent latching on external power on
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    Init_Pins ();
    Button_Press(); // interrupt sets Power_On_Flag to false

    while(1){
        if (!Check_Power_Flag()){
            Switch_Off ();
//            test_reset ();
//            __WFE();
        }
    }
}
