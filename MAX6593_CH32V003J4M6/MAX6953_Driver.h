#ifndef USER_MAX6953_DRIVER_H_
#define USER_MAX6953_DRIVER_H_

#define INTENSITY10_ADDRESS 0x01
#define INTENSITY32_ADDRESS 0x02
#define SCANLIMIT_ADDRESS 0x03
#define CONFIG_ADDRESS 0x04
#define USER_FONT_POINTER 0x05
#define DISPLAY_TEST_ADDRESS 0x07
#define DIGIT0_ADDRESS 0x20 // write Plane 0
#define DIGIT1_ADDRESS 0x21 // write Plane 0
#define DIGIT2_ADDRESS 0x22 // write Plane 0
#define DIGIT3_ADDRESS 0x23 // write Plane 0
#define P2_DIGIT0_ADDRESS 0x40 // write Plane 1
#define P2_DIGIT1_ADDRESS 0x41 // write Plane 1
#define P2_DIGIT2_ADDRESS 0x42 // write Plane 1
#define P2_DIGIT3_ADDRESS 0x43 // write Plane 1

/* Initialise I2C peripheral.*/
void i2C_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure={0};
    I2C_InitTypeDef I2C_InitStructure={0};

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE );
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_I2C1, ENABLE );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init( GPIOC, &GPIO_InitStructure );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init( GPIOC, &GPIO_InitStructure );

    I2C_InitStructure.I2C_ClockSpeed = 10000;
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_16_9;
    I2C_InitStructure.I2C_OwnAddress1 = 0x08;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init( I2C1, &I2C_InitStructure );

    I2C_Cmd( I2C1, ENABLE );
}

void Enable_i2c_Pullup(void)
{
    GPIO_InitTypeDef GPIO_InitStructure={0};

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init( GPIOA, &GPIO_InitStructure );

    GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_SET);      // enable pullup to pcb
}

/* Initialise each MAX6953.*/
void MAX_Init (u8 max_address)
{
    while( I2C_GetFlagStatus( I2C1, I2C_FLAG_BUSY ) != RESET );
    I2C_GenerateSTART( I2C1, ENABLE );

    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_MODE_SELECT ) );
    I2C_Send7bitAddress( I2C1, max_address << 1, I2C_Direction_Transmitter );

    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) );

    while( !I2C_GetFlagStatus( I2C1, I2C_FLAG_TXE ) );

    Delay_Ms(1);

    I2C_SendData( I2C1, 0x01 ); // control registers start address - auto increments address

    Delay_Ms(1);

    I2C_SendData( I2C1, BRIGHTNESS ); // intensity of digits 1 and 0

    Delay_Ms(1);

    I2C_SendData( I2C1, BRIGHTNESS );// intensity of digits 3 and 2

    Delay_Ms(1);

    I2C_SendData( I2C1, 0x01 ); // control 4 or 2 7x5 displays

    Delay_Ms(1);

    I2C_SendData( I2C1, 0x01 ); // config
    // 0000 0001 => 0 shutdown, 1 = normal mode, no fooling with blink modes & stuff
    // 0000 0010 = not used
    // 0000 0100 => 0 slow blink, 1 = fast blink
    // 0000 1000 => 0 = global blink disabled (Plane 1 data ignored), 1 global blink enabled Plane 1 data used)
    // 0001 0000 => 0 = blink unaffected during I2C acknowledge
    // 0010 0000 => 0 = data unaffected during I2C ack, 1 = data cleared
    // 0100 0000 => not used
    // 1000 0000 => blank phase - blink status is read back

    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );

    I2C_GenerateSTOP( I2C1, ENABLE );

}

void Display_Init (void)
{
    for ( u8 j = 0;  j < NUM_ICS; ++ j)
    {
        MAX_Init (MAX_6953 [j]);
    }
}

void BLINK_OFF (u8 max_address)
{
    while( I2C_GetFlagStatus( I2C1, I2C_FLAG_BUSY ) != RESET );
    I2C_GenerateSTART( I2C1, ENABLE );

    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_MODE_SELECT ) );
    I2C_Send7bitAddress( I2C1, max_address << 1, I2C_Direction_Transmitter );

    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) );

    while( !I2C_GetFlagStatus( I2C1, I2C_FLAG_TXE ) );

    Delay_Ms(1);

    I2C_SendData( I2C1, 0x04 ); // config register

    Delay_Ms(1);

    I2C_SendData( I2C1, 0x00 ); // all off

    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );

    I2C_GenerateSTOP( I2C1, ENABLE );

}

void BLINK_ON (u8 max_address)
{
    while( I2C_GetFlagStatus( I2C1, I2C_FLAG_BUSY ) != RESET );
    I2C_GenerateSTART( I2C1, ENABLE );

    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_MODE_SELECT ) );
    I2C_Send7bitAddress( I2C1, max_address << 1, I2C_Direction_Transmitter );

    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) );

    while( !I2C_GetFlagStatus( I2C1, I2C_FLAG_TXE ) );

    Delay_Ms(1);

    I2C_SendData( I2C1, 0x04 ); // config register

    Delay_Ms(1);

    I2C_SendData( I2C1, 0x01 ); // all on

    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );

    I2C_GenerateSTOP( I2C1, ENABLE );

}

void All_On (void)
{
    for ( u8 j = 0;  j < NUM_ICS; ++ j)
    {
        BLINK_ON (MAX_6953 [j]);
    }
}

void All_Off (void)
{
    for ( u8 j = 0;  j < NUM_ICS; ++ j)
    {
        BLINK_OFF (MAX_6953 [j]);
    }
}

/* Test each display.*/
void Display_Test (u8 max_address)
{
    while( I2C_GetFlagStatus( I2C1, I2C_FLAG_BUSY ) != RESET );

    I2C_GenerateSTART( I2C1, ENABLE );

    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_MODE_SELECT ) );

    I2C_Send7bitAddress( I2C1, max_address << 1, I2C_Direction_Transmitter );

    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) );

    while( !I2C_GetFlagStatus( I2C1, I2C_FLAG_TXE ) );

    I2C_SendData( I2C1, DISPLAY_TEST_ADDRESS );

    I2C_SendData( I2C1, 0x01 ); // 01 = display test on

    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );

    I2C_GenerateSTOP( I2C1, ENABLE );


    Delay_Ms(100);


    while( I2C_GetFlagStatus( I2C1, I2C_FLAG_BUSY ) != RESET );

    I2C_GenerateSTART( I2C1, ENABLE );

    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_MODE_SELECT ) );

    I2C_Send7bitAddress( I2C1, max_address << 1, I2C_Direction_Transmitter );

    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) );

    while( !I2C_GetFlagStatus( I2C1, I2C_FLAG_TXE ) );

    I2C_SendData( I2C1, DISPLAY_TEST_ADDRESS );

    I2C_SendData( I2C1, 0x00 ); // 01 = display test off

    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );

    I2C_GenerateSTOP( I2C1, ENABLE );
}

void Test_Display (void)
{
    for ( u8 j = 0;  j < NUM_ICS; ++ j)
    {
        Display_Test (MAX_6953 [j]);
    }
}

/* Update Displays.*/
void Update_Display (u8 max_character_0, u8 max_character_1, u8 max_character_2, u8 max_character_3, u8 max_address)
{
    u8 character_array [4] = {max_character_0,max_character_1,max_character_2,max_character_3};
    u8 digit_array [4] = {DIGIT0_ADDRESS,DIGIT1_ADDRESS,DIGIT2_ADDRESS,DIGIT3_ADDRESS};

    for (u8 i = 0; i < 4; ++i)
    {

        while( I2C_GetFlagStatus( I2C1, I2C_FLAG_BUSY ) != RESET );

        I2C_GenerateSTART( I2C1, ENABLE );

        while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_MODE_SELECT ) );

        I2C_Send7bitAddress( I2C1, max_address << 1, I2C_Direction_Transmitter );

        while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) );

        while( !I2C_GetFlagStatus( I2C1, I2C_FLAG_TXE ) );

        I2C_SendData( I2C1, digit_array [i] ); // register address

        I2C_SendData( I2C1, character_array [i] ); // character

        while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );

        I2C_GenerateSTOP( I2C1, ENABLE );

    }
}

void User_Font_Reset(u8 max_address) // fills first 12 user fonts with font_array data
{
    while(I2C_GetFlagStatus( I2C1, I2C_FLAG_BUSY ) != RESET );

    I2C_GenerateSTART( I2C1, ENABLE );

    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_MODE_SELECT ) );

    I2C_Send7bitAddress( I2C1, max_address << 1, I2C_Direction_Transmitter );

    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) );

    while( !I2C_GetFlagStatus( I2C1, I2C_FLAG_TXE ) );

    I2C_SendData( I2C1, 0xF7 ); // write to last register
    Delay_Ms(1);
    I2C_SendData( I2C1, 0x00 ); // write 0x00

    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );

    I2C_GenerateSTOP( I2C1, ENABLE );
}

void User_Font_Update(u8 max_address) // fills first 12 user fonts with font_array data
{
    int i = 0;
    int j = 0;

    while(I2C_GetFlagStatus( I2C1, I2C_FLAG_BUSY ) != RESET );

    I2C_GenerateSTART( I2C1, ENABLE );

    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_MODE_SELECT ) );

    I2C_Send7bitAddress( I2C1, max_address << 1, I2C_Direction_Transmitter );

    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) );

    while( !I2C_GetFlagStatus( I2C1, I2C_FLAG_TXE ) );

    I2C_SendData( I2C1, 0x05 ); // user font register pointer auto increments

    for (j = 0; j < 12; ++j)
    {
        for ( i = 0; i < 5; ++i)
        {
            I2C_SendData( I2C1, MY_FONT [j] [i] ); // character
            Delay_Ms(1);
        }
        Delay_Ms(1);
    }


    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );

    I2C_GenerateSTOP( I2C1, ENABLE );

    User_Font_Reset(max_address);

}

void User_Fonts_Update (void)
{
    for ( u8 j = 0;  j < NUM_ICS; ++ j)
    {
        User_Font_Update(MAX_6953 [j]);
    }
}


#endif /* USER_MAX6953_DRIVER_H_ */
