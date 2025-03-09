/********************************** (C) COPYRIGHT *******************************
 * File Name          : PD_TRIGGER_DRIVER.h
 * Author             : Green Dragon
 * Version            : V1.0.0
 * Date               : 11/2024
 * Description        : PD_Trigger PCB Driver
 * https://radicalbakers.org/green-dragon-maker-space
 ********************************************************************************/
#ifndef USER_PD_TRIGGER_DRIVER_H_
#define USER_PD_TRIGGER_DRIVER_H_

#include "debug.h"

void Button_Press(void);
void EXTI7_0_IRQHandler(void);
void LED_INIT(void);
void PD_CONFIG_INIT(void);
void POWERGOOD_INIT(void);
void ERROR_STATE(void);
uint8_t Check_Volts_Requested(void);
void REQ_5V(void);
void REQ_9V(void);
void REQ_12V(void);
void REQ_15V(void);
void REQ_20V(void);

#endif /* USER_PD_TRIGGER_DRIVER_H_ */
