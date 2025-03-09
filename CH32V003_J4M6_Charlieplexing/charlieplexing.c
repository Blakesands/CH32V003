/* charlieplexing.c Created on: 04/03/25 Author: BK
 * LEDs are controlled by setting pins as high, low, or high-impedance.
 * bool led_buffer holds the state of each LED. Only 1 LED is lit at a time...
 * Systick is used to update the LEDs at approx 10kHz */

#include <stdbool.h>
#include "animations.h"
#include "charlieplexing.h"

// in system_ch32v00x.c
// #define SYSCLK_FREQ_48MHZ_HSI   48000000

#define SYSTICK_FREQ 10000 // 10kHz

// offset into animation array
#define FRAME_ZERO 0
#define FRAME_ONE NUM_LEDS
#define FRAME_TWO NUM_LEDS*2
#define FRAME_THREE NUM_LEDS*3
#define FRAME_FOUR NUM_LEDS*4
#define FRAME_FIVE NUM_LEDS*5
#define FRAME_SIX NUM_LEDS*6
#define FRAME_SEVEN NUM_LEDS*7
#define FRAME_EIGHT NUM_LEDS*8
#define FRAME_NINE NUM_LEDS*9

// Global variable to keep track of the currently lit LED
volatile uint8_t current_led = 0;

// Buffer to store the LED states (0 = OFF, 1 = ON)
bool led_buffer[NUM_LEDS];

// Animation counter counted up by Systick at 10kHz, resets at LOOP_TIME
volatile uint32_t animation_counter = 0;

// Copy from animation to buffer, offset selects frame
void update_buffer(const bool src[], uint16_t offset) {
    NVIC_DisableIRQ(SysTicK_IRQn);
    memcpy(led_buffer, src+offset, NUM_LEDS * sizeof(bool));
    NVIC_EnableIRQ(SysTicK_IRQn);
}

// Which combination of pins light which LEDs, bottom section is shifted right in physical layout
uint8_t led_config[NUM_LEDS][2] = {
            {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, // 0-4
    {0, 1},         {2, 1}, {3, 1}, {4, 1}, {5, 1},  // 5-9
    {0, 2}, {1, 2},         {3, 2}, {4, 2}, {5, 2}, //10-14
    {0, 3}, {1, 3}, {2, 3},         {4, 3}, {5, 3}, //15-19
    {0, 4}, {1, 4}, {2, 4}, {3, 4},         {5, 4}, //20-24
    {0, 5}, {1, 5}, {2, 5}, {3, 5}, {4, 5}          //25-29
};

// PCB net to Pin mapping: 0:PC1, 1:PC2, 2:PC4, 3:PD4(also the swio), 4:PA1, 5:PA2

// define pins using HAL defines
const uint16_t led_pins[NUM_PINS] = {GPIO_Pin_1, GPIO_Pin_2, GPIO_Pin_4, GPIO_Pin_4, GPIO_Pin_1, GPIO_Pin_2};

// Define the port for each pin
GPIO_TypeDef *led_ports[NUM_PINS] = {GPIOC, GPIOC, GPIOC, GPIOD, GPIOA, GPIOA};

// init all the pins
void GPIO_Config(void){
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOD, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    for (int i = 0; i < NUM_PINS; i++) {
        GPIO_InitStructure.GPIO_Pin = led_pins[i];
        GPIO_Init(led_ports[i], &GPIO_InitStructure);
        GPIO_WriteBit(led_ports[i], led_pins[i], Bit_RESET);
    }
}

void set_pin_mode(uint8_t pin, int8_t mode){
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = led_pins[pin];

    if (mode == 0){
        // Output Low
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_Init(led_ports[pin], &GPIO_InitStructure);
        GPIO_WriteBit(led_ports[pin], led_pins[pin], Bit_RESET);
    }
    else if (mode == 1){
        // Output High
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_Init(led_ports[pin], &GPIO_InitStructure);
        GPIO_WriteBit(led_ports[pin], led_pins[pin], Bit_SET);
    }
    else {
        // High-Impedance (Input Floating)
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_Init(led_ports[pin], &GPIO_InitStructure);
    }
}

void update_led_pins(void){
    int first_pin = led_config[current_led][0];
    int second_pin = led_config[current_led][1];
    // Set all pins to high-impedance
    for (int i = 0; i < NUM_PINS; i++){
        set_pin_mode(i, -1);
    }
    // Check the state of the current LED in the buffer
    if (led_buffer[current_led]){
        // Turn ON the LED by setting the corresponding pins
        // If you have put the LEDs on backwards, reverse these :)
        set_pin_mode(first_pin, 1); // Set pin1 high
        set_pin_mode(second_pin, 0); // Set pin2 low
    }
    current_led++;
    if (current_led >= NUM_LEDS){
        current_led = 0; // Reset to the first LED
    }
}

void start_systick (void) { // Configure and enable SysTick
    SysTick->SR &= ~(1 << 0); // Clear the count flag
    SysTick->CMP = SystemCoreClock / SYSTICK_FREQ; // Interrupt every 100uS ish
    SysTick->CNT = 0;    // Reset the counter
    SysTick->CTLR = 0xF; // Enable SysTick, CPU clock, and interrupt (only first 3 bits used on ch32v003)
    NVIC_EnableIRQ(SysTicK_IRQn); // start updating pins
}

// SysTick interrupt handler
void SysTick_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void SysTick_Handler(void) {
    update_led_pins();
    animation_counter++;
    if (animation_counter>=LOOP_TIME) {
        animation_counter=0;
    }
    SysTick->SR = 0; // Clear the SysTick interrupt flag
}

// second variable in update_buffer is the offset into the animation array to select frame
void animate_frames (void){
    switch (animation_counter) {
        case FRAME9_TIME:
                    update_buffer(ANIMATION, FRAME_NINE);
            break;
        case FRAME8_TIME:
                    update_buffer(ANIMATION, FRAME_EIGHT);
            break;
        case FRAME7_TIME:
                    update_buffer(ANIMATION, FRAME_SEVEN);
            break;
        case FRAME6_TIME:
                    update_buffer(ANIMATION, FRAME_SIX);
            break;
        case FRAME5_TIME:
                    update_buffer(ANIMATION, FRAME_FIVE);
            break;
        case FRAME4_TIME:
                    update_buffer(ANIMATION, FRAME_FOUR);
            break;
        case FRAME3_TIME:
                    update_buffer(ANIMATION, FRAME_THREE);
            break;
        case FRAME2_TIME:
                    update_buffer(ANIMATION, FRAME_TWO);
            break;
        case FRAME1_TIME:
                    update_buffer(ANIMATION, FRAME_ONE);
            break;
        case FRAME0_TIME:
                    update_buffer(ANIMATION, FRAME_ZERO);
            break;
        default:
            break;
    }
}

// Clear the LED buffer (turn all LEDs off)
void clear_led_buffer(void) {
    memset(led_buffer, 0, sizeof(led_buffer));
}

// Fill the LED buffer (turn all LEDs on)
void all_leds_on(void) {
    memset(led_buffer, 1, sizeof(led_buffer));
}
// turn every X LEDs on
void every_x_leds_on(uint8_t every)
{
    for (uint8_t i = 0; i < NUM_LEDS; i++){
        if (i%every==0) {
            led_buffer[i] = 1;
        }
    }
}

