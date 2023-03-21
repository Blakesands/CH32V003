/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2022/08/08
 * Description        : Main program body.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/

/*
 *@Note
 single wire half duplex mode, master/slave mode, data transceiver:
 Master:SPI1_SCK(PC5)\SPI1_MOSI(PC6).
 Slave:SPI1_SCK(PC5)\SPI1_MISO(PC7).

 This routine demonstrates that Master sends and Slave receives.
 Note: The two boards download the Master and Slave programs respectively,
 and power on at the same time.
     Hardware connection:PC5 -- PC5
           PC7-- PC6

*/

#include "debug.h"

/* Global define */


//#define increment 0
//#define decrement 1

/* Global Variable */
volatile u8 Txval=0;

/* SPI Communication Mode Selection */
#define SPI_MODE   0

u8 TxData[2] = { 0x04, 0x08};


/*********************************************************************
 * @fn      SPI_1Lines_HalfDuplex_Init
 *
 * @brief   Configuring the SPI for half-duplex communication.
 *
 * @return  none */

 void SPI_1Lines_HalfDuplex_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure={0};
    SPI_InitTypeDef SPI_InitStructure={0};
    NVIC_InitTypeDef NVIC_InitStructure={0};

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC | RCC_APB2Periph_SPI1, ENABLE );




    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOC, &GPIO_InitStructure );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOC, &GPIO_InitStructure );

    SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;

    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init( SPI1, &SPI_InitStructure );

    NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    SPI_Cmd( SPI1, ENABLE );
}


 /*     GPIO_Mode_AIN = 0x0,
    GPIO_Mode_IN_FLOATING = 0x04,
    GPIO_Mode_IPD = 0x28,
    GPIO_Mode_IPU = 0x48,
    GPIO_Mode_Out_OD = 0x14,
    GPIO_Mode_Out_PP = 0x10,
    GPIO_Mode_AF_OD = 0x1C,
    GPIO_Mode_AF_PP = 0x18
 */

void cs_init(){
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void incrementZ(){
    GPIOC->BCR = 1 << 4; // put Chip Select pin C4 Low
    __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
    SPI_I2S_SendData( SPI1, TxData[0] );
Delay_Us(9);
    GPIOC->BSHR = 1 << 4; // put Chip Select pin C4 High
    __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
}

void decrementZ(){
    GPIOC->BCR = 1 << 4; // put Chip Select pin C4 Low
    __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
    SPI_I2S_SendData( SPI1, TxData[1] );
    Delay_Us(9);
    GPIOC->BSHR = 1 << 4; // put Chip Select pin C4 High
    __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
}


/*********************************************************************
 * @fn      main **/

int main(void)
{
//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    Delay_Init();
    cs_init();
    GPIOC->BSHR = 1 << 4; // put Chip Select pin C4 High
    SPI_1Lines_HalfDuplex_Init();
      Delay_Ms(1000);
//    GPIOC->BCR = 1 << 4; // put Chip Select pin C4 Low/
//    __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
//    Delay_Ms(1000);


//    Delay_Ms(1000);
//    SPI_I2S_ITConfig( SPI1, SPI_I2S_IT_TXE , ENABLE );

    while(1)
    {
        for (int i = 0; i < 130; ++i) {
            incrementZ();
            Delay_Ms(100);
        }
        Delay_Ms(1000);
        for (int i = 0; i < 130; ++i) {
            decrementZ();
            Delay_Ms(100);
        }
        Delay_Ms(1000);

    }
}
