/* Global defines */
#define FIVE 0
#define NINE 1
#define TWELVE 2
#define FIFTEEN 3
#define TWENTY 4

/* Global Variables */
  u8 V_REQ = FIVE;

  void  REQ_5V (void);

/** Enable Interrupt falling edge A2*/
void EXTI0_INT_INIT (void)
{
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

void EXTI7_0_IRQHandler (void) // changes V_REQ voltage requested variable
{
if(EXTI_GetITStatus(EXTI_Line2)!=RESET)
  {
    V_REQ++;
  if (V_REQ > TWENTY)
      {
      V_REQ = FIVE;
      }
//     Delay_Ms(50); // debounce?
  EXTI_ClearITPendingBit(EXTI_Line2);     /* Clear Flag */
  }
}

void LED_INIT (void) // output pin for addressable WS2812B LED
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void POWERGOOD_INIT (void) // input pin for powergood signal from CH224
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void ERROR_STATE (void)
{
    LED_SendColour(255, 0, 0, 100);
    Delay_Ms(150);
    LED_SendColour(0, 0, 0, 100);
    Delay_Ms(150);
    LED_SendColour(255, 0, 0, 100);
    Delay_Ms(150);
    LED_SendColour(0, 0, 0, 100);
    Delay_Ms(150);
    LED_SendColour(255, 0, 0, 100);
    Delay_Ms(150);
    LED_SendColour(0, 0, 0, 100);
    Delay_Ms(150);
    LED_SendColour(255, 0, 0, 100);
    Delay_Ms(150);
    LED_SendColour(0, 0, 0, 100);
    V_REQ = 0;
}

/// config pins from MCU to CH224
void  REQ_5V (void)
{
    GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_SET);      // cfg1
    GPIO_WriteBit(GPIOC, GPIO_Pin_2, Bit_RESET);    // cfg2
    GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_RESET);    // cfg3
    Delay_Ms(300);
    if (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1))
    {
        ERROR_STATE();
    }
    else
    {
        LED_SendColour(255, 0, 0, 100);
    }

}

void  REQ_9V  (void)
{
    GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_RESET);
    GPIO_WriteBit(GPIOC, GPIO_Pin_2, Bit_RESET);
    GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_RESET);
    Delay_Ms(300);
    if (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1))
        {
            ERROR_STATE();
        }
        else
        {
            LED_SendColour(0, 255, 0, 100);
        }
}

void  REQ_12V  (void)
{
    GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_RESET);
    GPIO_WriteBit(GPIOC, GPIO_Pin_2, Bit_RESET);
    GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_SET);
    Delay_Ms(300);
    if (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1))
        {
            ERROR_STATE();
        }
        else
        {
            LED_SendColour(0, 0, 255, 100);
        }
}

void  REQ_15V  (void)
{
    GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_RESET);
    GPIO_WriteBit(GPIOC, GPIO_Pin_2, Bit_SET);
    GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_SET);
    Delay_Ms(300);
    if (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1))
        {
            ERROR_STATE();
        }
        else
        {
            LED_SendColour(100, 75, 75, 100);
        }
}

void  REQ_20V  (void)
{
    GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_RESET);
    GPIO_WriteBit(GPIOC, GPIO_Pin_2, Bit_SET);
    GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_RESET);
    Delay_Ms(300);
    if (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1))
        {
            ERROR_STATE();
        }
        else
        {
            LED_SendColour(255, 0, 255, 100);
        }
}

void  PD_CONFIG_INIT (void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 |GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}
