/********************************** (C) COPYRIGHT *******************************
 * File Name          : PD_TRIGGER_DRIVER.c
 * Author             : Green Dragon
 * Version            : V1.0.0
 * Date               : 03/2025
 * Description        : PD_Trigger PCB Driver
 * https://radicalbakers.org/green-dragon-maker-space
 ********************************************************************************/

#include "PD_TRIGGER_DRIVER.h"

// Addressable LED Driver
#include "PD_TRIGGER_WS2812_DRIVER.h"

volatile uint8_t V_REQ = 0;

uint8_t Check_Volts_Requested(){
    uint8_t temp = V_REQ;
    return temp;
}

/** Enable Interrupt falling edge A2*/
void Button_Press (void){
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    EXTI_InitTypeDef EXTI_InitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* GPIOA ----> EXTI_Line2 */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource2);
    EXTI_InitStructure.EXTI_Line = EXTI_Line2;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI7_0_IRQn; // external hardware interrupts
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void EXTI7_0_IRQHandler (void) __attribute__((interrupt("WCH-Interrupt-fast")));

void EXTI7_0_IRQHandler (void) { // changes V_REQ variable
    if(EXTI_GetITStatus(EXTI_Line2)!=RESET){
        V_REQ++;
        if (V_REQ > 4){
            V_REQ = 0;
        }
        EXTI_ClearITPendingBit(EXTI_Line2);     /* Clear Flag */
    }
}

void LED_INIT (void) { // output pin for addressable WS2812B LED
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    LED_SendColour(0, 0, 0, 100); // resets LED
}

void  PD_CONFIG_INIT (void) { // control pins for cfg CH224
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 |GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void POWERGOOD_INIT (void){ // input pin for powergood signal from CH224
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void ERROR_STATE (void) { // Flash led and reset V_REQ - falls through to WFI
    GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_SET);      // cfg1
    GPIO_WriteBit(GPIOC, GPIO_Pin_2, Bit_RESET);    // cfg2
    GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_RESET);    // cfg3
    LED_SendColour(0, 0, 0, 100);
    LED_SendColour(255, 0, 0, 100);
    Delay_Ms(150);
    LED_SendColour(0, 0, 0, 100);
    Delay_Ms(150);
    LED_SendColour(255, 0, 0, 60);
    Delay_Ms(150);
    LED_SendColour(0, 0, 0, 100);
    Delay_Ms(150);
    LED_SendColour(255, 0, 0, 30);
    Delay_Ms(150);
    LED_SendColour(0, 0, 0, 100);
    Delay_Ms(150);
    LED_SendColour(255, 0, 0, 10);
    V_REQ = 4; // updates to 0 on next button press
    __enable_irq ();
}

/// config pins from MCU to CH224
void  REQ_5V (void){
    __disable_irq ();
    GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_SET);      // cfg1
    GPIO_WriteBit(GPIOC, GPIO_Pin_2, Bit_RESET);    // cfg2
    GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_RESET);    // cfg3
    Delay_Ms(300);
    if (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1)){   // check power good signal from CH224
        ERROR_STATE();
    }
    else{
        LED_SendColour(255, 0, 0, 100);
    }
    __enable_irq ();
}

void  REQ_9V  (void){
    __disable_irq ();
    GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_RESET);
    GPIO_WriteBit(GPIOC, GPIO_Pin_2, Bit_RESET);
    GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_RESET);
    Delay_Ms(300);
    if (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1)){
        ERROR_STATE();
    }
    else{
        LED_SendColour(0, 255, 0, 100);
    }
    __enable_irq ();
}

void  REQ_12V  (void){
    __disable_irq ();
    GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_RESET);
    GPIO_WriteBit(GPIOC, GPIO_Pin_2, Bit_RESET);
    GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_SET);
    Delay_Ms(300);
    if (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1)){
        ERROR_STATE();
    }
    else{
        LED_SendColour(0, 0, 255, 100);
    }
    __enable_irq ();
}

void  REQ_15V  (void){
    __disable_irq ();
    GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_RESET);
    GPIO_WriteBit(GPIOC, GPIO_Pin_2, Bit_SET);
    GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_SET);
    Delay_Ms(300);
    if (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1)){
        ERROR_STATE();
    }
    else{
        LED_SendColour(100, 75, 75, 100);
    }
    __enable_irq ();
}

void  REQ_20V  (void){
    __disable_irq ();
    GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_RESET);
    GPIO_WriteBit(GPIOC, GPIO_Pin_2, Bit_SET);
    GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_RESET);
    Delay_Ms(300);
    if (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1)){
        ERROR_STATE();
    }
    else{
        LED_SendColour(255, 0, 255, 100);
    }
    __enable_irq ();
}
