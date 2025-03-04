// Include necessary libraries
#include <stdlib.h>

// Define an array to buffer the colour data for each LED in the strip
uint8_t BUFFER_LEDS[num_leds][3] = {};

// Initialise the variable for the srand seed
uint16_t ADC_val;

// Initialise the variable for the TP4056 charge indicator (for battery powered systems)
int chargeflag = 0;

void EXTI0_INT_INIT(void){
    EXTI_InitTypeDef EXTI_InitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    /* GPIOA ----> EXTI_Line0 */
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

// handles EXTI0
void EXTI7_0_IRQHandler(void){
  if(EXTI_GetITStatus(EXTI_Line1)!=RESET){
   int chargeflag = 1;
    EXTI_ClearITPendingBit(EXTI_Line1);     /* Clear Flag */
  }
}

// Initialise the GPIO pin GPIOC GPIO_Pin_4 as an output
void DATA_INIT(void){
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

// Initialise the GPIO pin GPIOC GPIO_Pin_1 as an input
void INPUT_INIT(void){
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void RANDSEED_INIT(void){
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    ADC_InitTypeDef ADC_InitStructure = {0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
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
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_3Cycles);
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
}

// Utility
int Map_Range(int value, int inMin, int inMax, int outMin, int outMax) {
    // Make sure the input value is within the input range
    value = value < inMin ? inMin : value;
    value = value > inMax ? inMax : value;
    // Map the value from the input range to the output range
    return (value - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}

// Send a single bit, Check mark/space ratio of the data on C4 with an oscilloscope
void LED_SendBit(uint8_t bit){
    if (bit) {
    //// Send a 1 bit
        GPIOC->BSHR = 1 << 4; // put pin C4 high and wait for 800nS
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop"); __asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop"); __asm__("nop");__asm__("nop");

        GPIOC->BCR = 1 << 4; // put pin C4 low and exit, 400nS is taken up by other functions
//        __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
//        __asm__("nop");__asm__("nop"); __asm__("nop");__asm__("nop");

        return;
        }
        // Send a 0 bit
        GPIOC->BSHR = 1 << 4; // put pin C4 high and wait for 400nS
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");

        GPIOC->BCR = 1 << 4; // put pin C4 low and wait for 400nS, 400nS is taken up by other functions
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
//            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
//            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
}

// Send a single colour for a single LED, WS2812B LEDs want 24 bits per led in the string
void LED_SendColour(uint8_t Red, uint8_t Green, uint8_t Blue, uint8_t brightness){
    uint8_t green = Map_Range(Green, 0, 255, 0, (brightness-100)); // -100 adjusting for red led low forward voltage
    uint8_t red = Map_Range(Red, 0, 255, 0, brightness);
    uint8_t blue = Map_Range(Blue, 0, 255, 0, (brightness-100));
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
void SHOWTIME(uint8_t BUFFER_LEDS[num_leds][3]){
    // send LED info
    for (int i = 0; i < num_leds; i++) {
        LED_SendColour(BUFFER_LEDS[i][0], BUFFER_LEDS[i][1], BUFFER_LEDS[i][2], BRIGHT);
    }
    // Delay for minimum 50uS between pinwiggles to signal next frame to WS2812B LEDS
    Delay_Us(50);
}
