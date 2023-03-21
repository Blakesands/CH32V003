/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : Green Dragom Maker Space
 * Version            : V1.0.0
 * Date               : 21/03/23
 * Description        : Driver for MCP4131 Microchip Digital Potentiometer
*******************************************************************************/
/*
Single wire half duplex mode
CS (PC4) SCK(PC5) MOSI(PC6).
*/

#include "debug.h"

u8 TxData[2] = { 0x04, 0x08}; // 0x04 is the 8 bit increment command,  0x08 is the 8 bit decrement command

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

int main(void)
{
    Delay_Init();
    cs_init();
    GPIOC->BSHR = 1 << 4; // put Chip Select pin C4 High
    SPI_1Lines_HalfDuplex_Init();
    Delay_Ms(1000);

    while(1) // slowly changes resistance for testing
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
