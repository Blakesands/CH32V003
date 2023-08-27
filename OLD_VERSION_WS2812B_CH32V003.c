// Include necessary libraries
#include "debug.h"  // For debugging and printing
#include <stdlib.h>
#include <math.h> // for rand and srand functions

// Define the number of LEDs in the strip
#define num_leds 140

// Define the number of steps for the sine table
#define NUM_STEPS 256

// defines the pin used for KEY0 button
#define KEY0 GPIO_ReadInputDataBit( GPIOC, GPIO_Pin_1)

// Define an array to hold the last color data of the LEDs
uint8_t last_color[3];

// Initialize the variables for animation
u32 count = 10;
int flagZ = 0;

// variable for brightness/speed
u8 brightness = 254;

//variable for srand seed
uint16_t ADC_val;

// variable for switch case in main loop
int STATEZ = 0;

// Define an array to buffer the color data for each LED in the strip
uint8_t BUFFER_LEDS[num_leds][3] = {};

// Define an array as a look up table for dimming to a sine pattern
u8 sine_lookup[256] = {
    0, 0, 1, 2, 3, 4, 5, 6, 8, 9, 11, 13, 15, 17, 20, 22, 25, 28, 31, 34,
    37, 40, 43, 46, 49, 53, 56, 60, 64, 67, 71, 75, 79, 83, 87, 91, 95, 99,
    103, 107, 111, 115, 119, 123, 126, 130, 133, 137, 140, 143, 146, 150, 153,
    156, 159, 162, 165, 168, 171, 174, 177, 180, 182, 185, 188, 190, 193, 195,
    198, 200, 202, 204, 206, 208, 210, 212, 214, 216, 217, 219, 220, 222, 223,
    224, 226, 227, 228, 229, 230, 231, 232, 233, 233, 234, 235, 235, 236, 236,
    237, 237, 237, 237, 237, 238, 238, 238, 238, 238, 238, 238, 238, 239, 239,
    239, 239, 239, 239, 240, 242, 244, 246, 248, 250, 250, 248, 246, 244, 242,
    240, 239, 239, 239, 239, 239, 239, 238, 238, 238, 238, 238, 238, 238, 238,
    238, 237, 237, 237, 237, 237, 236, 236, 235, 235, 234, 233, 233, 232, 231,
    230, 229, 228, 227, 226, 224, 223, 222, 220, 219, 217, 216, 214, 212, 210,
    208, 206, 204, 202, 200, 198, 195, 193, 190, 188, 185, 182, 180, 177, 174,
    171, 168, 165, 162, 159, 156, 153, 150, 146, 143, 140, 137, 133, 130, 126,
    123, 119, 115, 111, 107, 103, 99, 95, 91, 87, 83, 79, 75, 71, 67, 64, 60,
    56, 53, 49, 46, 43, 40, 37, 34, 31, 28, 25, 22, 20, 17, 15, 13, 11, 9, 8,
    6, 5, 4, 3, 2, 1, 0, 0
};

// "Sine" Wave lookup table
const u8 sine_lookup_2[256] = {
      0, 5, 6, 7, 8, 9,
      12, 15, 18, 20, 23, 26, 29, 31,
      34, 37, 40, 43, 45, 48, 51, 54,
      57, 60, 63, 66, 68, 71, 74, 77,
      80, 83, 86, 89, 91, 94, 97, 100,
      103, 106, 109, 112, 115, 118, 121, 124,
      127, 130, 133, 136, 139, 142, 145, 148,
      151, 154, 157, 160, 163, 166, 169, 172,
      174, 177, 180, 183, 186, 189, 192, 195,
      197, 200, 203, 206, 209, 211, 214, 217,
      220, 222, 225, 228, 230, 233, 235, 238,
      240, 243, 245, 247, 250, 252, 254, 255,
      255, 255, 255, 255, 255, 254, 252, 250,
      247, 245, 243, 240, 238, 235, 233, 230,
      228, 225, 222, 220, 217, 214, 211, 209,
      206, 203, 200, 197, 195, 192, 189, 186,
      183, 180, 177, 174, 172, 169, 166, 163,
      160, 157, 154, 151, 148, 145, 142, 139,
      136, 133, 130, 127, 124, 121, 118, 115,
      112, 109, 106, 103, 100, 97, 94, 91,
      89, 86, 83, 80, 77, 74, 71, 68,
      66, 63, 60, 57, 54, 51, 48, 45,
      43, 40, 37, 34, 31, 29, 26, 23,
      20, 18, 15, 12,
      9, 6, 7, 6, 5, 0
};

u8 Sine_Wave[256] = {
    50, 53, 56, 59, 62, 65, 68, 71, 74, 77, 80, 83, 86, 89, 92, 95,
    98, 101, 104, 107, 110, 113, 116, 119, 122, 125, 128, 131, 134, 137, 140, 143,
    146, 149, 152, 155, 158, 161, 164, 167, 170, 173, 176, 179, 182, 185, 188, 191,
    194, 197, 200, 203, 206, 209, 212, 215, 218, 221, 224, 227, 230, 233, 236, 239,
    242, 245, 248, 251, 254, 255, 254, 251, 248, 245, 242, 239, 236, 233, 230, 227,
    224, 221, 218, 215, 212, 209, 206, 203, 200, 197, 194, 191, 188, 185, 182, 179,
    176, 173, 170, 167, 164, 161, 158, 155, 152, 149, 146, 143, 140, 137, 134, 131,
    128, 125, 122, 119, 116, 113, 110, 107, 104, 101, 98, 95, 92, 89, 86, 83,
    80, 77, 74, 71, 68, 65, 62, 59, 56, 53, 50
};
//
//// Smooth Ramp lookup table
u8 ramp_lookup[256] = {
    0, 1, 2, 3, 5, 6, 7, 9, 10, 12, 13, 15, 16, 18, 20, 21, 23, 24, 26, 28,
    29, 31, 33, 34, 36, 38, 39, 41, 43, 45, 46, 48, 50, 52, 53, 55, 57, 59,
    60, 62, 64, 66, 68, 69, 71, 73, 75, 77, 78, 80, 82, 84, 86, 88, 89, 91,
    93, 95, 97, 99, 101, 102, 104, 106, 108, 110, 112, 114, 116, 118, 120,
    122, 124, 126, 128, 130, 132, 134, 136, 138, 140, 142, 144, 146, 148,
    150, 152, 154, 156, 158, 160, 162, 165, 167, 169, 171, 173, 175, 177, 180,
    182, 184, 186, 189, 191, 193, 195, 198, 200, 202, 205, 207, 209, 212,
    214, 216, 219, 221, 223, 226, 228, 231, 233, 236, 238, 241, 243, 246,
    248, 251, 253, 255
};

//// Fairly random Lookup Table
u8 rand_lookup[256] = {
  233, 253, 182, 90, 165, 191, 249, 161, 40, 207, 172, 157, 118, 166, 197, 159,
  191, 60, 226, 30, 65, 173, 154, 91, 67, 12, 205, 87, 53, 92, 194, 121,
  17, 56, 191, 20, 25, 181, 195, 98, 54, 85, 123, 246, 198, 167, 31, 197,
  66, 102, 14, 75, 39, 87, 207, 144, 67, 98, 187, 87, 186, 28, 127, 56,
  4, 66, 239, 54, 170, 106, 177, 56, 107, 63, 10, 98, 228, 18, 50, 138,
  65, 79, 120, 231, 42, 70, 199, 115, 47, 71, 152, 91, 178, 200, 182, 60,
  129, 44, 86, 118, 61, 251, 94, 210, 32, 143, 168, 131, 129, 212, 87, 142,
  121, 223, 162, 187, 216, 61, 163, 166, 54, 97, 101, 25, 12, 219, 52, 205,
  98, 127, 13, 30, 29, 157, 90, 191, 244, 8, 30, 123, 63, 66, 198, 154,
  28, 236, 165, 64, 156, 35, 63, 126, 198, 146, 60, 117, 77, 80, 2, 16,
  73, 25, 28, 18, 34, 181, 60, 181, 119, 149, 68, 218, 182, 68, 235, 112,
  91, 48, 0, 131, 195, 139, 16, 76, 32, 89, 45, 13, 3, 32, 97, 234,
  78, 154, 45, 236, 142, 235, 73, 236, 226, 50, 167, 201, 93, 50, 181, 145,
  24, 3, 72, 52, 15, 44, 134, 0, 0, 86, 57, 174, 20, 78, 5, 0,
  26, 10, 0, 31, 219, 0, 0, 70, 71, 0, 31,
};
//'Not-Random_at-All' lookup table weighted with 255 and 0
u8 rand_lookup_2[256] = {
  255, 255, 255, 0, 170, 170, 255, 170, 85, 255, 170, 170, 85, 170, 170, 85,
  191, 60, 226, 0, 65, 173, 154, 91, 67, 0, 205, 87, 53, 92, 194, 121,
  0, 0, 255, 0,0, 181, 255, 98, 54, 85, 123, 255, 255, 167, 0, 197,
  66, 102, 0, 75, 0, 87, 207, 144, 0, 98, 255, 0, 255, 0, 127, 56,
  0, 0, 239, 0, 170, 106, 177, 56, 107, 63, 0, 98, 228, 18, 50, 138,
  65, 79, 120, 231, 0, 70, 199, 115, 0, 0, 170, 85, 170, 255, 170, 60,
  129, 0, 85, 118, 85, 251, 85, 255, 32, 143, 168, 131, 129, 212, 87, 142,
  121, 255, 0, 0, 255, 61, 255, 255, 54, 97, 170, 0, 0, 219, 52, 255,
  255, 170, 0, 0, 0, 157, 90, 255, 255, 0, 30, 123, 63, 66, 198, 154,
  28, 236, 165, 64, 156, 0, 0, 126, 198, 146, 60, 117, 77, 80, 2, 16,
  73, 0, 0, 18, 34, 0, 60, 181, 119, 255, 0, 218, 255, 68, 235, 112,
  91, 48, 0, 255, 255, 139, 16, 76, 32, 89, 45, 13, 0, 32, 97, 234,
  78, 154, 45, 236, 142, 235, 73, 236, 226, 50, 167, 201, 93, 50, 181, 145,
  24, 3, 72, 52, 15, 44, 134, 0, 0, 255, 57, 255, 20, 255, 5, 0,
  26, 10, 0, 31, 219, 0, 0, 70, 71, 0, 31,
};

void init_array (void){ // fills BUFFER_LEDS with random values seeded from ADC reading a floating pin A4
GPIO_InitTypeDef GPIO_InitStructure = {0};
ADC_InitTypeDef ADC_InitStructure = {0};

RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
GPIO_Init(GPIOA, &GPIO_InitStructure);

/* Enable ADC1 clock */
RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
/* ADC1 Configuration */
//    ADC_InitTypeDef ADC_InitStructure;
ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
ADC_InitStructure.ADC_ScanConvMode = DISABLE;
ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
ADC_InitStructure.ADC_NbrOfChannel = 1;
ADC_Init(ADC1, &ADC_InitStructure);
/* Enable ADC1 */
ADC_Cmd(ADC1, ENABLE);
//    ADC_Channel_Vrefint(ENABLE);
ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_15Cycles);
// Start the ADC conversion
ADC_SoftwareStartConvCmd(ADC1, ENABLE);
// Wait for the conversion to complete
while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
// Get the ADC conversion value
ADC_val = ADC_GetConversionValue(ADC1);
// End the ADC conversion
ADC_SoftwareStartConvCmd(ADC1, DISABLE);
/* Disable ADC1 */
ADC_Cmd(ADC1, DISABLE);

//    random seed is ADC reading of floating pin
srand((unsigned) ADC_val);
    for (int i = 0; i < num_leds; ++i) {

            BUFFER_LEDS[i][0] = rand_lookup_2[(rand() % 256)]; // % modulus function
            BUFFER_LEDS[i][1] = rand_lookup_2[(rand() % 256)];
            BUFFER_LEDS[i][2] = rand_lookup_2[(rand() % 256)];
        }
}

// Initialize the GPIO wiggle pin
void GPIO_INITZ(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

// initialises external interrupt and sets it to pin C1, enables interrupt
void EXTI0_INT_INIT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    EXTI_InitTypeDef EXTI_InitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* GPIOC ----> EXTI_Line1 */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource1);
    EXTI_InitStructure.EXTI_Line = EXTI_Line1;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = 12;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void EXTI7_0_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

// interrupt handler increments STATEZ variable
void EXTI7_0_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line1)!=RESET)
  {
      STATEZ++;
          if (STATEZ > 6) {
                 STATEZ=0;
              }
    EXTI_ClearITPendingBit(EXTI_Line1);     /* Clear Flag */
  }
}

// Send a single bit using the GD pinwiggle protocol.
// These ratios can change with different code. Check mark/space ratio of the data on C4 with an oscilloscope
void LED_SendBit(uint8_t bit)
{
    if (bit) {
    //// Send a 1 bit
        GPIOC->BSHR = 1 << 4; // put pin C4 high and wait for 800ns
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");
        GPIOC->BCR = 1 << 4; // put pin C4 low and exit, the rest of the 1.2us is taken up by other functions
        return;
        }
//    else {
        // Send a 0 bit
        GPIOC->BSHR = 1 << 4; // put pin C4 high and wait for 400ns
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");
        GPIOC->BCR = 1 << 4; // put pin C4 low and wait intil 1.2us
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
//    }
}

// Send a single colour for a single LED
//WS2812B LEDs want 24 bits per led in the string
void LED_SendColour(uint8_t red, uint8_t green, uint8_t blue)
{
    // Send the green component first (MSB)
    for (int i = 7; i >= 0; i--) {
        LED_SendBit((green >> i) & 1);
    }
    // Send the red component next
    for (int i = 7; i >= 0; i--) {
        LED_SendBit((red >> i) & 1);
    }
    // Send the blue component last (LSB)
    for (int i = 7; i >= 0; i--) {
        LED_SendBit((blue >> i) & 1);
    }
}
// Send the colour data for the entire strip of LEDs
// TODO add brightness control
void SHOWTIME(uint8_t BUFFER_LEDS[num_leds][3])
{
    //    LED_OFF();
    NVIC_DisableIRQ(EXTI7_0_IRQn);
    // send LED info
    for (int i = 0; i < num_leds; i++) {
        LED_SendColour(BUFFER_LEDS[i][0], BUFFER_LEDS[i][1], BUFFER_LEDS[i][2]);
    }
    NVIC_EnableIRQ(EXTI7_0_IRQn);
    // Delay for minimum 50uS between pinwiggles to signal next frame to WS2812B LEDS
    Delay_Us(50);
}


void LED_OFF(void)
{
    for (int i = 0; i <  151; i++) {
        LED_SendColour(0, 0, 0);
    }
//    Delay_Ms(333);
}

void LED_RED(void)
{
    for (int i = 0; i < num_leds; i++) {
        LED_SendColour(255, 0, 0);
    }
    Delay_Ms(333);
}

void LED_GREEN(void)
{
    for (int i = 0; i < num_leds; i++) {
        LED_SendColour(0, 255, 0);
    }
    Delay_Ms(333);
}

void LED_BLUE(void)
{
    for (int i = 0; i < num_leds; i++) {
        LED_SendColour(0, 0, 255);
    }
    Delay_Ms(333);
}

void white_static(void)
{
    for (int i = 0; i < num_leds; i++) {
        LED_SendColour(255, 220, 210);
    }
    Delay_Ms(1);
}

void rgb_flash(void)
{
    count++;
    if (count > 5) {
        count = 0;
    }
    switch (count) {
        case 0:
            LED_RED();

            break;
        case 1:
            LED_OFF();
            Delay_Ms(333);

            break;
        case 2:
            LED_GREEN();

            break;
        case 3:
            LED_OFF();
            Delay_Ms(333);

            break;
        case 4:
            LED_BLUE();

            break;
        case 5:
            LED_OFF();
            Delay_Ms(333);

            break;
        default:
            break;
    }
}

// Shift the colours of the LEDs by one position and save the colour of the last LED
void animate_leds(void)
{
//    LED_OFF();
    uint8_t LEDS[num_leds][3] = {  };
// Set the first LED to the saved colour of the last LED
BUFFER_LEDS[0][0] = last_color[0];
BUFFER_LEDS[0][1] = last_color[1];
BUFFER_LEDS[0][2] = last_color[2];

// Shift the colours of the LEDs by one position
for (int i = num_leds - 1; i > 0; i--) {
    LEDS[i][0] = BUFFER_LEDS[i - 1][0];
    LEDS[i][1] = BUFFER_LEDS[i - 1][1];
    LEDS[i][2] = BUFFER_LEDS[i - 1][2];
//    BUFFER_LEDS[0][0] = BUFFER_LEDS[num_leds - 1][0];
//    BUFFER_LEDS[0][1] = BUFFER_LEDS[num_leds - 1][1];
//    BUFFER_LEDS[0][2] = BUFFER_LEDS[num_leds - 1][2];
    BUFFER_LEDS[i][0] = LEDS[i][0];
    BUFFER_LEDS[i][1] = LEDS[i][1];
    BUFFER_LEDS[i][2] = LEDS[i][2];
}
// Save the colour of the last LED
last_color[0] = BUFFER_LEDS[num_leds - 1][0];
last_color[1] = BUFFER_LEDS[num_leds - 1][1];
last_color[2] = BUFFER_LEDS[num_leds - 1][2];
if (!(last_color[2]&&last_color[1])) { ///Yay! for logical operators
//    if (last_color[1]==0) {
      last_color[0] = 255;
    }
BUFFER_LEDS[0][0] = last_color[0] ;
BUFFER_LEDS[0][1] = last_color[1] ;
BUFFER_LEDS[0][2] = last_color[2] ;
SHOWTIME(BUFFER_LEDS);
Delay_Ms(brightness);
}

void rand_animate_leds(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    ADC_InitTypeDef ADC_InitStructure = {0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    /* Enable ADC1 clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    /* ADC1 Configuration */
    //    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);
    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);
    //    ADC_Channel_Vrefint(ENABLE);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_15Cycles);
    // Start the ADC conversion
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    // Wait for the conversion to complete
    while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
    // Get the ADC conversion value
    ADC_val = ADC_GetConversionValue(ADC1);
    // End the ADC conversion
    ADC_SoftwareStartConvCmd(ADC1, DISABLE);
    /* Disable ADC1 */
    ADC_Cmd(ADC1, DISABLE);
    //    random seed is ADC reading of floating pin
    srand((unsigned) ADC_val);
    //    LED_OFF();
    uint8_t LEDS[num_leds][3] = {};
    //    srand((unsigned) ADC_val);
    // Set the first LED to a random colour
    BUFFER_LEDS[0][0] = sine_lookup[(rand() % 15)];
    BUFFER_LEDS[0][1] = sine_lookup[(rand() % 30)];
    BUFFER_LEDS[0][2] = sine_lookup[(rand() % 75)];
    if (!(BUFFER_LEDS[0][1]&&BUFFER_LEDS[0][2])) { ///Yay! for logical operators
        BUFFER_LEDS[0][0] = 255;
        }
    // Shift the colours of the LEDs by one position
    for (int i = num_leds - 1; i > 0; i--) {
        LEDS[i][0] = BUFFER_LEDS[i - 1][0];
        LEDS[i][1] = BUFFER_LEDS[i - 1][1];
        LEDS[i][2] = BUFFER_LEDS[i - 1][2];
    //    BUFFER_LEDS[0][0] = BUFFER_LEDS[num_leds - 1][0];
    //    BUFFER_LEDS[0][1] = BUFFER_LEDS[num_leds - 1][1];
    //    BUFFER_LEDS[0][2] = BUFFER_LEDS[num_leds - 1][2];
        BUFFER_LEDS[i][0] = LEDS[i][0];
        BUFFER_LEDS[i][1] = LEDS[i][1];
        BUFFER_LEDS[i][2] = LEDS[i][2];
    }
    SHOWTIME(BUFFER_LEDS);
    Delay_Ms(100);
}

// This function applies the sine wave to the LED colours and updates the buffer
void animate_leds_sine(void)
{
// Increment count for sine wave animation
    if (flagZ==0) {
        count++;
    }
    if (flagZ==1) {
        count--;
        }
    if (count >= 65) {
        count = 65;
    flagZ = 1;
    }
    if (count <= 10) {
        count = 10;
        flagZ = 0;
        }
// Apply the sine wave to the LED colors
for (int i = 0; i < num_leds; i++) {
    // Update the LED colour values with the mapped sine wave value
    BUFFER_LEDS[i][0] = sine_lookup[count]; // set red value to sine wave value
    BUFFER_LEDS[i][1] = sine_lookup[count]; // set green value to sine wave value
    BUFFER_LEDS[i][2] = sine_lookup[count]; // set blue value to sine wave value
}
SHOWTIME(BUFFER_LEDS);
Delay_Ms(10);
}

void animate_leds_color(void) {

    // Linear interpolation function
    uint8_t lerp(uint8_t a, uint8_t b, float t) {
        return (uint8_t)((1.0 - t) * a + t * b);
    }
    // Define two colors to interpolate between
    const uint8_t COLOR1[3] = {0, 255, 0}; // Red
    const uint8_t COLOR2[3] = {255,0, 0}; // Blue
    // Increase the count variable for the animation
    count++;
    if (count >= 255) {
        count = 0;
    }

    // Calculate the interpolation factor (range: 0.0 to 1.0)
    float t = ramp_lookup[count]; // Use a sine wave for a smooth animation

    // Loop through each LED in the strip
    for (int i = 0; i < num_leds; i++) {
        // Interpolate between COLOR1 and COLOR2 based on the current count value
        uint8_t red = lerp(COLOR1[0], COLOR2[0], t);
        uint8_t green = lerp(COLOR1[1], COLOR2[1], t);
        uint8_t blue = lerp(COLOR1[2], COLOR2[2], t);

        // Update the buffer with the new color values
        BUFFER_LEDS[i][0] = red;
        BUFFER_LEDS[i][1] = green;
        BUFFER_LEDS[i][2] = blue;
    }

    // Show the updated buffer on the LED strip
    SHOWTIME(BUFFER_LEDS);

    // Wait for a short period of time before updating the colors again
    Delay_Ms(200);
}




// The main function initialises the GPIO and USART, sets the initial color values
// of the LED strip, and loops forever animating the LED strip.
int main(void)
{
// Initialise delay, GPIO
Delay_Init();
GPIO_INITZ(); // configures pin C4 as Output Push/Pull for data out
//EXTI0_INT_INIT(); // inits keypress interrupt

// Send initial LED colour values to the LED strip
LED_OFF();
init_array();

// Main loop
while (1) {
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//
        switch (STATEZ) {
            case 0:
                rand_animate_leds();

                break;
            case 1:
                LED_RED();

                break;
            case 2:
                LED_GREEN();

                break;
            case 3:
                LED_BLUE();

                break;
            case 4:
                white_static();

                break;
            case 5:
                rgb_flash();

                break;
            case 6:
                animate_leds_sine();

                break;
            default:

                break;
        } // end of switch case
    } // end of while
} // end of Main
