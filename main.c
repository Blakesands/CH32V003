/*****************************************************************
 * File Name          : main.c
 * Author             : Green Dragon Maker Space
 * Date               : 20/02/2023
 * Web                : https://radicalbakers.org/green-dragon-maker-space/microcontroller
 ********************************************************************
 This code is for the Green Dragon CH32V003F4P6 Microcontroller Breakout Board
 It implements a variable duty cycle PWM LED MOSFET dimmer with buttons and current sensing.
 The system can monitor the current across the MOSFET and pull the MOS gate low to shut off the PWM signal
 if an over-current condition is detected. It will resume with previous settings if the over-current condition is removed.

Function Descriptions:

The KEY_Init function initialises two buttons (KEY0 and KEY1) by configuring GPIO pins C1 and C2 as input with
internal pull-down resistors. The KEY_PRESS and KEY_PRESS_1 functions read the state of the buttons, de-bounce them
and return 1 if a button is pressed, 0 otherwise.

The TIM1_PWMOut function initialises Timer 1 and outputs a 19.8kHz variable duty cycle PWM signal on GPIO pin D2.

The OPA1_Init function initialises an operational amplifier (OPA1) by configuring GPIO pins D7 as the OP+ and
D0 as OP- inputs and then enabling the OPA with output on D4 which is the ADC input channel 7. The OPA Inverting input must be
externally connected via a resistor to OPA Output and connected to ground via a resistor to create a non-inverting amplifier
for current sensing across the MOSFET.

The ADC_Channel7_Init function initialises analogue-to-digital converter (ADC) channel 7. The Get_ADC_Val and Get_ADC_Average
functions read the ADC value and calculate the average of multiple readings.

The Monitor function is called periodically from the SysTicK_IRQ interrupt handler and reads the current sensing ADC value
to check for an over-current condition. If an over-current condition is detected, the PWM signal is disabled and a Warning Overcurrent
message is printed to the console. If the current returns within the acceptable range, the PWM signal is enabled, and a status: Good
message is printed.

Main(void) calls the functions that initialise various parts of the system, including GPIO pins, timers and interrupts.

While(1) continuously loops and, if the OC flag is not set, checks for key presses. It then adjusts the PWM duty cycle and therefore
the LED brightness based on the button pressed.

/* includes here */
#include "debug.h"

/* Global define */
#define KEY0 GPIO_ReadInputDataBit( GPIOC, GPIO_Pin_1)  // defines the pin used for KEY0 button
#define KEY1 GPIO_ReadInputDataBit( GPIOC, GPIO_Pin_2)  // defines the pin used for KEY1 button

/* Global Variables */
u8 STATE = 10;
//u16 ADC_val;
//u8 OC_flag = 0;
//volatile uint32_t timer_count = 0;

void KEY_Init(void) // initializes the pins used for KEY0 and KEY1 buttons
{
    GPIO_InitTypeDef GPIO_InitStructure={0};

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init( GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init( GPIOC, &GPIO_InitStructure);
}

u8 KEY_PRESS(void) // checks if KEY0 button is pressed and returns a value based on that
{
    if(KEY0 == 1)
    {
        Delay_Ms(10);
        if(KEY0 == 1) return 1;
        else return 0;
    }
    return 0;
}

u8 KEY_PRESS_1(void) // checks if KEY1 button is pressed and returns a value based on that
{
    if(KEY1 == 1)
    {
        Delay_Ms(10);
        if(KEY1 == 1) return 1;
        else return 0;
    }
    return 0;
}

void OPA1_Init( void ) // initialises the operational amplifier (OPA)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    OPA_InitTypeDef  OPA_InitStructure = {0};

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD, ENABLE );
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init( GPIOD, &GPIO_InitStructure );
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init( GPIOD, &GPIO_InitStructure );

    OPA_InitStructure.PSEL = CHP1;
    OPA_InitStructure.NSEL = CHN1;
    OPA_Init( &OPA_InitStructure );
    OPA_Cmd( ENABLE );
}

void ADC_Channel7_Init( void ) // initializes analog-to-digital converter (ADC) channel 7 that is connected to the OPA output
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    ADC_InitTypeDef ADC_InitStructure = {0};

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD, ENABLE );
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_ADC1, ENABLE );
    RCC_ADCCLKConfig( RCC_PCLK2_Div8 );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOD, &GPIO_InitStructure );

    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init( ADC1, &ADC_InitStructure );

    ADC_Calibration_Vol(ADC1, ADC_CALVOL_50PERCENT);
    ADC_Cmd( ADC1, ENABLE );

    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));
}

u16 Get_ADC_Val( u8 ch ) // gets the value of the ADC channel
{
    u16 val;
    ADC_RegularChannelConfig( ADC1, ch, 1, ADC_SampleTime_241Cycles );
    ADC_SoftwareStartConvCmd( ADC1, ENABLE );
    while( !ADC_GetFlagStatus( ADC1, ADC_FLAG_EOC ) );
    val = ADC_GetConversionValue( ADC1 );
    return val;
}

u16 Get_ADC_Average( u8 ch, u8 times ) // gets the average value of the ADC channel over a specified number of times
{
    u32 temp_val = 0;
    u8 t;
    u16 val;
    for( t = 0; t < times; t++ ){
        temp_val += Get_ADC_Val( ch );
        Delay_Ms( 5 );
    }
    val = temp_val / times;
    return val;
}

//void monitor () // monitors the system for overcurrent, disables or enables PWM, pulls pin low and prints out status messages
//{
//    NVIC_DisableIRQ(SysTicK_IRQn);
//    int i_sense = Get_ADC_Average( ADC_Channel_7, 6 );
////    printf( "Current Sensing\r\n" );
////    printf( "ADC Reading:%d\r\n", i_sense );
//    if (i_sense >= 950) { // Overcurrent condition
//        TIM_Cmd( TIM1, DISABLE );
//        GPIO_InitTypeDef GPIO_InitStructure={0};
//        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
//        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//        GPIO_Init( GPIOD, &GPIO_InitStructure );
//        GPIO_WriteBit(GPIOC, GPIO_Pin_2, Bit_RESET);
//        OC_flag = 1;
//        printf( "WARNING OVERCURRENT\r\n");
//    } // end of overcurrent
//    if (i_sense <=949) {
//        if (OC_flag == 1) { // Clear Overcurrent condition
//                TIM_Cmd( TIM1, ENABLE );
//                GPIO_InitTypeDef GPIO_InitStructure={0};
//                GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
//                GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//                GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//                GPIO_Init( GPIOD, &GPIO_InitStructure );
//                OC_flag = 0;
//                printf( "Status: Good\r\n");
//        } // end of if
//    } // end of status good
//} // end of monitor

// Outputs PWM signal on D2
// *         `    arr - the period value.
// *              psc - the prescaler value.
// *              ccp - the pulse value.
void TIM1_PWMOut(u16 arr, u16 psc, u16 ccp)
{
    GPIO_InitTypeDef GPIO_InitStructure={0};
    TIM_OCInitTypeDef TIM_OCInitStructure={0};
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure={0};

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD | RCC_APB2Periph_TIM1, ENABLE );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOD, &GPIO_InitStructure );

    TIM_TimeBaseInitStructure.TIM_Period = arr;
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit( TIM1, &TIM_TimeBaseInitStructure);

    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = ccp;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init( TIM1, &TIM_OCInitStructure );

    TIM_CtrlPWMOutputs(TIM1, ENABLE );
    TIM_OC1PreloadConfig( TIM1, TIM_OCPreload_Disable );
    TIM_ARRPreloadConfig( TIM1, ENABLE );
    TIM_Cmd( TIM1, ENABLE );
}

//void SysTick_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
//
////void SysTick_Handler(void)
//{
//if (timer_count >= 10) {
//    timer_count = 0;
//    monitor();
//}
//timer_count++;
//}

void init_NVIC(void){
    SysTick->SR &= ~(1 << 0);
    SysTick->CMP = SystemCoreClock;
    SysTick->CNT = 1;
    SysTick->CTLR = 0xF;
}

int main(void)
{
    Delay_Init();
    KEY_Init();
    USART_Printf_Init( 115200 );
//    OPA1_Init();
//    ADC_Channel7_Init();
    printf( "SystemClk:%d\r\n", SystemCoreClock );
    printf( "Green Dragon_LED_Dimmer\r\n" );
//    init_NVIC();
//    NVIC_EnableIRQ(SysTicK_IRQn);
    Delay_Ms(500);
    TIM1_PWMOut( 100, 24-1, STATE );

    while(1)
    {
//        NVIC_EnableIRQ(SysTicK_IRQn);
//        if (OC_flag == 0) {
            if( KEY_PRESS() == 1 )  //PC1
                    {
//                NVIC_DisableIRQ(SysTicK_IRQn);
                    STATE--;
                    if (STATE<=1) {
                            STATE=1;
                    } // end of limiting if
                        TIM1_PWMOut( 100, 24-1, STATE );
                        Delay_Ms(25);
             } // end of key press 1
            if( KEY_PRESS_1() == 1 )  //PC2
                    {
//                NVIC_DisableIRQ(SysTicK_IRQn);
                STATE++;
                    if (STATE>=25) {
                        STATE=25;
                    } // end of limiting if
                        TIM1_PWMOut( 100, 24-1, STATE );
                        Delay_Ms(25);
             } // end of key press 2
//        } // end of if
//        else {
//            Delay_Ms(100);
////            monitor();
//        }

    } // end of While
} // end of Main
