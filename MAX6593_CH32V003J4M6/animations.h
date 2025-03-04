/*
 *  animations.h
 *  Created on: 9 Nov 2024
 *  Author: BK
 */

#ifndef USER_ANIMATIONS_H_
#define USER_ANIMATIONS_H_

void Update_Display (u8 max_character_0, u8 max_character_1, u8 max_character_2, u8 max_character_3, u8 max_address);
void All_On (void);
void All_Off (void);



void Fade_Down (int max_address)
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

void Fade_All (void)
    {
        for ( u8 j = 0;  j < NUM_ICS; ++ j)
        {
            Fade_Down (MAX_6953 [j]);
        }
    }

void Blank_Display ()
{

    Update_Display ( 0x20, 0x20, 0x20, 0x20, (MAX_6953 [0]) );
    Delay_Ms(1);
    Update_Display ( 0x20, 0x20, 0x20, 0x20, (MAX_6953 [1]) );
    Delay_Ms(1);
    Update_Display ( 0x20, 0x20, 0x20, 0x20, (MAX_6953 [2]) );
    Delay_Ms(1);

}

void Bye ()
{
    char Character_Map_1 [6] = {' ', 'B', 'Y', 'E', '!' ,' ' };
    char Character_Map_2 [6] = { ' ', ' ', ' ', ' ', ' ',' ' };

    Update_Display ( 0x20, 0x20, 0x20, 0x20, (MAX_6953 [0]) );
    Delay_Ms(1);
    Update_Display ( 0x20, 0x20, 0x20, 0x20, (MAX_6953 [1]) );
    Delay_Ms(1);
    Update_Display ( 0x20, 0x20, 0x20, 0x20, (MAX_6953 [2]) );
    Delay_Ms(1);

    for (int dim = 0; dim < 5; ++dim)
    {
        Update_Display ( Character_Map_1 [0], Character_Map_1 [1], Character_Map_2 [0], Character_Map_2 [1], (MAX_6953 [0]) );
        Delay_Ms(1);
        Update_Display ( Character_Map_1 [2], Character_Map_1 [3], Character_Map_2 [2], Character_Map_2 [3], (MAX_6953 [1]) );
        Delay_Ms(1);
        Update_Display ( Character_Map_1 [4], Character_Map_1 [5], Character_Map_2 [4], Character_Map_2 [5], (MAX_6953 [2]) );
        BRIGHTNESS = BRIGHTNESS - 0x11;
        Fade_All ();
        Delay_Ms(150);
    }
    Blank_Display();

}

void Hello_World ()
{
    char Character_Map_1 [6] = {' ', 'H', 'i', '!', ' ' ,' ' };
    char Character_Map_2 [6] = { ' ', ' ', ' ', ' ', ' ',' ' };

    Update_Display ( 0x20, 0x20, 0x20, 0x20, (MAX_6953 [0]) );
    Delay_Ms(1);
    Update_Display ( 0x20, 0x20, 0x20, 0x20, (MAX_6953 [1]) );
    Delay_Ms(1);
    Update_Display ( 0x20, 0x20, 0x20, 0x20, (MAX_6953 [2]) );
    Delay_Ms(1);

    BRIGHTNESS = 0x00;

    for (int dim = 0; dim < 5; ++dim)
    {
        Fade_All ();
        Update_Display ( Character_Map_1 [0], Character_Map_1 [1], Character_Map_2 [0], Character_Map_2 [1], (MAX_6953 [0]) );
        Delay_Ms(1);
        Update_Display ( Character_Map_1 [2], Character_Map_1 [3], Character_Map_2 [2], Character_Map_2 [3], (MAX_6953 [1]) );
        Delay_Ms(1);
        Update_Display ( Character_Map_1 [4], Character_Map_1 [5], Character_Map_2 [4], Character_Map_2 [5], (MAX_6953 [2]) );
        BRIGHTNESS = BRIGHTNESS + 0x11;
        Fade_All ();
        Delay_Ms(250);
    }
    Delay_Ms(250);

}



void Dont_Panic (u16 speed)
{
    u8 i = 0;
//    BRIGHTNESS = 0x11;
    char Character_Map_5 [12] = { 'D', 'o', 'n', 0x27, 't', ' ', 'Y', 'o', 'u', ' ', ' ', ' '};
    char Character_Map_6 [12] = { 'P','a', 'n', 'i', 'c', ' ','M', 'u', 'p', 'p','e', 't',};

    Update_Display ( Character_Map_5 [i], Character_Map_5 [i+1], Character_Map_6 [i], Character_Map_6 [i+1], (MAX_6953 [0]) );
    Delay_Ms(1);
    Update_Display ( Character_Map_5 [i+2], Character_Map_5 [i+3], Character_Map_6 [i+2], Character_Map_6 [i+3], (MAX_6953 [1]) );
    Delay_Ms(1);
    Update_Display ( Character_Map_5 [i+4], Character_Map_5 [i+5], Character_Map_6 [i+4], Character_Map_6 [i+5], (MAX_6953 [2]) );
    All_On();
    Delay_Ms(speed*3);

    for (i = 0; i < 7; i++)
    {
        Update_Display ( Character_Map_5 [i], Character_Map_5 [i+1], Character_Map_6 [i], Character_Map_6 [i+1], (MAX_6953 [0]) );
        Delay_Ms(1);
        Update_Display ( Character_Map_5 [i+2], Character_Map_5 [i+3], Character_Map_6 [i+2], Character_Map_6 [i+3], (MAX_6953 [1]) );
        Delay_Ms(1);
        Update_Display ( Character_Map_5 [i+4], Character_Map_5 [i+5], Character_Map_6 [i+4], Character_Map_6 [i+5], (MAX_6953 [2]) );
        Delay_Ms(speed);
    }

    Delay_Ms(speed*2);
    All_Off ();
    Delay_Ms(speed*2);
}

void Static_12ch_Sign (void)
{
    //char Character_Map_1 [6] = { '!', '?', '.', ',', '/', '+' };
    //char Character_Map_2 [6] = { '=', '-', '@', '%', '&', '(' };
//    BRIGHTNESS = 0xdd;

//    char myString[] = "This is some text";

    char Character_Map_1 [6] = { 'L', 'o', 'v', 'e', 'l', 'y'};
    char Character_Map_2 [6] = { ' ','B', 'e', 'e', 'r', ' ' };

    Update_Display ( Character_Map_1 [0], Character_Map_1 [1], Character_Map_2 [0], Character_Map_2 [1], (MAX_6953 [0]) );
    Delay_Ms(1);
    Update_Display ( Character_Map_1 [2], Character_Map_1 [3], Character_Map_2 [2], Character_Map_2 [3], (MAX_6953 [1]) );
    Delay_Ms(1);
    Update_Display ( Character_Map_1 [4], Character_Map_1 [5], Character_Map_2 [4], Character_Map_2 [5], (MAX_6953 [2]) );
}

void Static_12ch_User_Fonts (void)
{
//    BRIGHTNESS = 0x11;
    u8 User_Fonts_1 [6] = { 0x00,0x01,0x02,0x03,0x04,0x05 };
    u8 User_Fonts_2 [6] = { 0x06,0x07,0x08,0x09,0x0a,0x0b };

    Update_Display ( User_Fonts_1 [0], User_Fonts_1  [1], User_Fonts_2  [0], User_Fonts_2  [1], (MAX_6953 [0]) );
    Delay_Ms(1);
    Update_Display ( User_Fonts_1 [2], User_Fonts_1  [3], User_Fonts_2  [2], User_Fonts_2  [3], (MAX_6953 [1]) );
    Delay_Ms(1);
    Update_Display ( User_Fonts_1 [4], User_Fonts_1  [5], User_Fonts_2  [4], User_Fonts_2  [5], (MAX_6953 [2]) );
}

void Increment_Intensity_Display (u8 max_address)
{
    while( I2C_GetFlagStatus( I2C1, I2C_FLAG_BUSY ) != RESET );
    I2C_GenerateSTART( I2C1, ENABLE );

    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_MODE_SELECT ) );
    I2C_Send7bitAddress( I2C1, max_address, I2C_Direction_Transmitter );

    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) );

    while( !I2C_GetFlagStatus( I2C1, I2C_FLAG_TXE ) );

    Delay_Ms(1);

    I2C_SendData( I2C1, 0x01 ); // control registers start address - auto increments address

    Delay_Ms(1);

    I2C_SendData( I2C1, BRIGHTNESS); // intensity of digits 1 and 0

    Delay_Ms(10);

    I2C_SendData( I2C1, BRIGHTNESS );// intensity of digits 3 and 2

    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );

    I2C_GenerateSTOP( I2C1, ENABLE );
}

void Fade_12ch_Sign (void)
{
    char Character_Map_1 [6] = { 'D', 'o', 'n', 0x27, 't', ' '};
    char Character_Map_2 [6] = { 'P','a', 'n', 'i', 'c', ' ' };

    BRIGHTNESS = 0x00;

    Update_Display ( Character_Map_1 [0], Character_Map_1 [1], Character_Map_2 [0], Character_Map_2 [1], (MAX_6953 [0]) );
    Delay_Ms(1);
    Update_Display ( Character_Map_1 [2], Character_Map_1 [3], Character_Map_2 [2], Character_Map_2 [3], (MAX_6953 [1]) );
    Delay_Ms(1);
    Update_Display ( Character_Map_1 [4], Character_Map_1 [5], Character_Map_2 [4], Character_Map_2 [5], (MAX_6953 [2]) );

    while (1)
    {
        BRIGHTNESS=BRIGHTNESS + 0x11;
        if (BRIGHTNESS == 0x77)
        {
            BRIGHTNESS = 0x00;
        }
        for ( u8 j = 0;  j < NUM_ICS; ++ j)
            {
                Increment_Intensity_Display (MAX_6953 [j]);
            }

        Delay_Ms(500);
    }
}

int user_font_array_2 [12][5] = {
{0b00000000,
 0b00000000,
 0b00000000,
 0b00000000,
 0b00000000},

{
 0b01111110,
 0b01000010,
 0b01000010,
 0b00111100,
 0b00000000},

{
 0b00111100,
 0b01000010,
 0b01000010,
 0b00111100,
 0b00000000},

{
 0b01111110,
 0b00000100,
 0b00001000,
 0b01111110,
 0b00000000},

{0b00000100,
 0b00000100,
 0b01111110,
 0b00000100,
 0b00000100},

{0b00000000,
 0b00000000,
 0b00000000,
 0b00000000,
 0b00000000},



 {0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000},

{0b00111110,
 0b00001010,
 0b00001010,
 0b00000100,
 0b00000000},

{
 0b00111100,
 0b00001010,
 0b00001010,
 0b00111100,
 0b00000000},

 {
  0b00111110,
  0b00000100,
  0b00001000,
  0b00111110,
  0b00000000},

{
 0b00111010,
 0b00000000,
 0b00011100,
 0b00100010,
 0b00100010},

{0b00010100,
 0b00000000,
 0b00000000,
 0b00000000,
 0b00000000},

};

int user_font_array_1 [12][5] = {
{0b00000000,
 0b00000000,
 0b00000000,
 0b00000000,
 0b00000000},

{
 0b00111111,
 0b00100001,
 0b00100001,
 0b00011110,
 0b00000000},

{
 0b00011100,
 0b00100010,
 0b00100010,
 0b00011100,
 0b00000000},

{
 0b00111110,
 0b00000100,
 0b00001000,
 0b00111110,
 0b00000000},

{0b00000010,
 0b00000101,
 0b00000100,
 0b00111111,
 0b00000100},

{0b00000100,
 0b00000000,
 0b00000000,
 0b00000000,
 0b00000000},



 {0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000},

{0b00111111,
 0b00001001,
 0b00001001,
 0b00000110,
 0b00000000},

{
 0b00111100,
 0b00001010,
 0b00001010,
 0b00111100,
 0b00000000},

 {
  0b00111110,
  0b00000100,
  0b00001000,
  0b00111110,
  0b00000000},

{
 0b00111010,
 0b00000000,
 0b00011110,
 0b00100001,
 0b00100001},

{0b00010010,
 0b00000000,
 0b00000000,
 0b00000000,
 0b00000000},

};

#endif /* USER_ANIMATIONS_H_ */
