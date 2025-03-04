/* CH32V003 J4M6 Soft latching switch with power off communication to main system
 * pins:
 * 1. PA1       Nmos gate drive
 *              Pu
 * 2.           Ground 0V
 * 3.           NC
 * 4.           VDD +5V
 * 5. PC1       Indicator LED
 *
 * 6. PC2       Button Press sense
 *
 * 7. PC4       AUX output, signal back to host
 * 8. PD4       SWIO programming
 * */

/* Includes */

#include "debug.h"

/* Global Defines */


/* Global Variables */

volatile uint8_t Power_Off_Flag = 0x01;

void Latch_Nmos () // immediately pull latch pin high
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_SET);
}

void Host_Comms_Pin()
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_SET);
    Delay_Ms(250);
    GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_RESET);
    Delay_Ms(250);
    GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_SET);
    Delay_Ms(250);
    GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_RESET);
    Delay_Ms(250);
    GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_SET);
}

void Host_Comms()
{
    GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_RESET);
}

void LED_Pin ()
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_SET);
}



void Switch_Off ()
{
//    NVIC_InitTypeDef NVIC_InitStructure = {0}; // disables interrupts in this scope
//    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
//    NVIC_Init(&NVIC_InitStructure);
//    ISR Control Routines from ch32fun ???
//            __enable_irq();    // For global interrupt enable

//            __isenabled_irq(); // For seeing if interrupts are enabled.
//            NVIC_EnableIRQ(IRQn_Type IRQn) // To enable a specific interrupt



    __disable_irq();   //  global interrupt disable
    Host_Comms(); // pulls pin low to signal back to host

    for (uint8_t i = 0; i < 6; i++) // flash the LED
    {


        if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1))
        {
            GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_RESET);
        }

        else
        {
            GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_SET);
        }

        Delay_Ms(250);
    }

    GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_RESET); // LED off
    GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_RESET); // pull Nmos gate low

    GPIO_InitTypeDef GPIO_InitStructure = {0}; // set Nmos pin to IPD with any remaining power
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

}


/** Enable Interrupt falling edge C2*/
void EXTI0_INT_INIT (void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    EXTI_InitTypeDef EXTI_InitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* GPIOC ----> EXTI_Line2 */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource2);
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

void EXTI7_0_IRQHandler (void) // changes V_REQ variable
{
    if(EXTI_GetITStatus(EXTI_Line2)!=RESET)
    {
        Power_Off_Flag--;
        EXTI_ClearITPendingBit(EXTI_Line2);     /* Clear Flag */
    }
}

//void test_reset ()
//{
//    Delay_Ms(3500);
//    Power_Off_Flag = 0x01;
//    GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_SET); // turn power LED back on
//
//    GPIO_InitTypeDef GPIO_InitStructure = {0}; // set Nmos pin back to Out_PP
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_Init(GPIOA, &GPIO_InitStructure);
//
//    // Set host comms pin back to Out_PP
////    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
////    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
////    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
////    GPIO_Init(GPIOD, &GPIO_InitStructure);
//
//    GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_SET); // pull Nmos gate back high
//    GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_SET); // pull Host comms high
//}

int main(void)
{

    SystemCoreClockUpdate();
    Latch_Nmos(); // immediately latch Nmos gate high on A1
    Host_Comms_Pin(); // Host comms high on D4
    LED_Pin(); // Power indicator LED on C1

    Delay_Init();
    Delay_Ms(250);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    EXTI0_INT_INIT(); // Button_Press

    while(1)
    {
        if (Power_Off_Flag < 0x01)
        {
            Switch_Off ();
//            test_reset ();

            __WFE();
        }
    }
}
