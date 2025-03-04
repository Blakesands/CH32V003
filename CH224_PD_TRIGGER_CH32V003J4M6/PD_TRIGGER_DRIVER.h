/* Variables */
u8 V_REQ = 0;

/** Enable Interrupt falling edge A2*/
void EXTI0_INT_INIT (void){
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

void EXTI7_0_IRQHandler (void){ // changes V_REQ variable
    if(EXTI_GetITStatus(EXTI_Line2)!=RESET){
        V_REQ++;
        if (V_REQ > 4){
            V_REQ = 0;
        }
        EXTI_ClearITPendingBit(EXTI_Line2);     /* Clear Flag */
    }
}

// output pin for addressable WS2812B LED
void LED_INIT (void) { 
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}

// control pins for cfg CH224
void  PD_CONFIG_INIT (void) { 
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 |GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}
 // input pin for powergood signal from CH224
void POWERGOOD_INIT (void){
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}
// Flash led and reset V_REQ - falls through to WFI
void ERROR_STATE (void) {
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
    Delay_Ms(150);
    LED_SendColour(0, 0, 0, 100);
    V_REQ = 4;
}

/// config pins from MCU to CH224
void  REQ_5V (void){
    GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_SET);      // cfg1
    GPIO_WriteBit(GPIOC, GPIO_Pin_2, Bit_RESET);    // cfg2
    GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_RESET);    // cfg3
    Delay_Ms(300);
     // check power good signal from CH224
    if (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1))  {
        ERROR_STATE();
    }
    else{
        LED_SendColour(255, 0, 0, 100);
    }
}

void  REQ_9V  (void){
    GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_RESET);
    GPIO_WriteBit(GPIOC, GPIO_Pin_2, Bit_RESET);
    GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_RESET);
    Delay_Ms(300);
    if (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1){
        ERROR_STATE();
    }
    else{
        LED_SendColour(0, 255, 0, 100);
    }
}

void  REQ_12V  (void){
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
}

void  REQ_15V  (void){
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
}

void  REQ_20V  (void){
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
}
