/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2022/08/08
 * Description        : Main program body.
*********************************************************************************
This program is to control a digital MCP4012 50k potentiometer.
It has 2 functions: Increment and Decrement
*******************************************************************************/

#include "debug.h"


/* Global define */


/* Global Variable */


void pins_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    // C4 will be Chip Select CS pin
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    // D4 will be digipot_data
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

}

/*********************************************************************

 */
void decrement_resistance(int times)
{
        GPIOD->BSHR = 1 << 4; // put digipot data pin D4 high and wait for 500ns
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
        GPIOC->BCR = 1 << 4; // put Chip Select pin C4 Low with 3us settling time
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
for (int i = 0; i < times; ++i) {
        GPIOD->BCR = 1 << 4; // put digipot data pin D4 low and wait for 500ns
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");

        GPIOD->BSHR = 1 << 4; // put digipot data pin D4 high and wait for 500ns
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");

    }
        GPIOC->BSHR = 1 << 4; // put Chip Select pin C4 High
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
        GPIOD->BCR = 1 << 4; // put digipot data pin D4 low and wait for 10us before next increment
}

void increment_resistance(int times)
{

        GPIOD->BCR = 1 << 4; // begin with digipot data pin D4 low and wait for 500ns
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
        GPIOC->BCR = 1 << 4; // put Chip Select pin C4 Low with 3us settling time
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
    for (int i = 0; i < times; ++i) {
        GPIOD->BSHR = 1 << 4; // put digipot data pin D4 high and wait for 500ns
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
        GPIOD->BCR = 1 << 4; // put digipot data pin D4 low and wait for 500ns
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");

    }

    GPIOC->BSHR = 1 << 4; // put Chip Select pin C4 High
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");

}

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    Delay_Init();
    pins_init();
    GPIOC->BCR = 1 << 4; // put Chip Select pin C4 High
    Delay_Ms(100);
    GPIOD->BSHR = 1 << 4; // put digipot data pin D4 low
    decrement_resistance(64);

    while(1)
    {  int i =0;
       for (i = 0; i > 63; ++i) {
           increment_resistance(1);
           Delay_Ms(100);
    }
       for (i = 0; i >63 ; ++i) {

       decrement_resistance(1);
       Delay_Ms(500);
       }
    }
}
