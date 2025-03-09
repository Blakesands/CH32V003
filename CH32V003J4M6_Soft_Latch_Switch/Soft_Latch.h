/* Soft_Latch.h Created on: 9 Mar 2025 Author: BK */

#ifndef USER_SOFT_LATCH_H_
#define USER_SOFT_LATCH_H_

#include "debug.h"
#include <stdbool.h>

/* pins:
 * 1. PA1       Nmos gate drive
 * 2.           Ground 0V
 * 3.           NC
 * 4.           VDD +5V
 * 5. PC1       Indicator LED
 * 6. PC2       Button Press sense
 * 7. PC4       AUX output, signal back to host
 * 8. PD4       SWIO programming
 * */

void Init_Pins();
void Latch_Nmos();
void Host_Comms_Pin();
void LED_Pin();
void Button_Press(void);
void EXTI7_0_IRQHandler(void);
bool Check_Power_Flag();
void Switch_Off();
void Host_Comms();
void test_reset();

#endif /* USER_SOFT_LATCH_H_ */
