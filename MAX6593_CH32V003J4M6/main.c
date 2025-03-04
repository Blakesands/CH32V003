/*
 * File Name          : main.c
 * Author             : Green Dragon
 * Version            : V1.0.0
 * Date               : 11/24
 * Description        : CH32V003 I2c driver for MAX6593, SCL(PC2) SDA(PC1).
*/

#include "debug.h"

// control multiple MAX6953 ICs

#define NUM_ICS 3 // number of MAX6953 ICs
#define MY_FONT user_font_array_1 // user_font_array_1
u8 MAX_6953 [NUM_ICS] = {0x50, 0x51, 0x52}; // 01010000 01010001 01010010 7 bit Addresses << 1
u8 BRIGHTNESS = 0x55; // each half-byte controls different digits - set to 0x11, 0x22 etc..
u8 Power_Off_Flag = 0x01;
#include "animations.h"
#include "MAX6953_Driver.h"

/** Enable Interrupt falling edge A2*/
void EXTI0_INT_INIT (void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    EXTI_InitTypeDef EXTI_InitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* GPIOA ----> EXTI_Line2 */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource4);
    EXTI_InitStructure.EXTI_Line = EXTI_Line4;
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

void EXTI7_0_IRQHandler (void) // changes V_REQ variable
{
    if(EXTI_GetITStatus(EXTI_Line4)!=RESET)
    {
        Bye ();
//        Delay_Ms(1500);
        EXTI_ClearITPendingBit(EXTI_Line4);     /* Clear Flag */
    }
}

int main(void)
{
    SystemCoreClockUpdate();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    Delay_Init();
    Enable_i2c_Pullup();
    Delay_Ms(10);
    i2C_Init();
    Delay_Ms(10);
    Display_Init();
    Delay_Ms(10);
    Test_Display();
    Delay_Ms(10);
    Blank_Display ();
    Delay_Ms(10);
    User_Fonts_Update();
    Delay_Ms(100);
    EXTI0_INT_INIT ();
    Hello_World ();
    Static_12ch_User_Fonts();
    while(1)
    {
//        Dont_Panic(500); // animated (speed in mS)
//        Static_12ch_User_Fonts();
//        All_On ();
//        Static_12ch_Sign();
//        Fade_12ch_Sign ();
//        __WFI();
    }
}
