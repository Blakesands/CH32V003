/* charlieplexing.h Created on: 04/03/25 Author: BK
 * LEDs are controlled by setting pins as high, low, or high-impedance.
 * bool led_buffer holds the state of each LED. Only 1 LED is lit at a time...
 * Systick is used to update the LEDs at approx 10kHz */

#ifndef USER_CHARLIEPLEXING_H_
#define USER_CHARLIEPLEXING_H_

#include <string.h>

// Pin to pcb net mapping: 0:PC1, 1:PC2, 2:PC4, 3:PD4(also the swio), 4:PA1, 5:PA2
const uint16_t led_pins[NUM_PINS] = {GPIO_Pin_1, GPIO_Pin_2, GPIO_Pin_4, GPIO_Pin_4, GPIO_Pin_1, GPIO_Pin_2};

// Define the ports associated with the pins
GPIO_TypeDef *led_ports[NUM_PINS] = {GPIOC, GPIOC, GPIOC, GPIOD, GPIOA, GPIOA};

// Global variable to keep track of the currently lit LED
volatile uint8_t current_led = 0;

// Buffer to store the LED states (0 = OFF, 1 = ON)
bool led_buffer[NUM_LEDS];

// Animation control
volatile uint32_t animation_frame_counter = 0;

// Which combination of pins light which LEDs, bottom section is shifted right in physical layout
int led_config[NUM_LEDS][2] = {
            {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, // 0-4
    {0, 1},         {2, 1}, {3, 1}, {4, 1}, {5, 1},  // 5-9
    {0, 2}, {1, 2},         {3, 2}, {4, 2}, {5, 2}, //10-14
    {0, 3}, {1, 3}, {2, 3},         {4, 3}, {5, 3}, //15-19
    {0, 4}, {1, 4}, {2, 4}, {3, 4},         {5, 4}, //20-24
    {0, 5}, {1, 5}, {2, 5}, {3, 5}, {4, 5} //25-29
};

// Copy frame to buffer
void update_buffer(const bool src[]) {
    memcpy(led_buffer, src, NUM_LEDS * sizeof(bool));
}

// Clear the LED buffer (turn all LEDs off)
void clear_led_buffer(void) {
    memset(led_buffer, 0, sizeof(led_buffer));
}

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

void start_systick (void) { // Configure and enable SysTick
    SysTick->SR &= ~(1 << 0); // Clear the count flag
    SysTick->CMP = SystemCoreClock / (10000); // Interrupt every 100nS ish
    SysTick->CNT = 0;    // Reset the counter
    SysTick->CTLR = 0xF; // Enable SysTick, CPU clock, and interrupt (only first 3 bits used on ch32v003)
    NVIC_EnableIRQ(SysTicK_IRQn); // start updating pins
}

void set_pin_mode(int pin, int mode){
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
    // Set all pins to high-impedance initially
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

// SysTick interrupt handler
void SysTick_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void SysTick_Handler(void) {
    update_led_pins();
    animation_frame_counter++;
    if (animation_frame_counter==NUMBER_FRAMES) {
        animation_frame_counter=0;
    }
    SysTick->SR = 0; // Clear the SysTick interrupt flag
}

void animate_frames (void){
    if (animation_frame_counter==FRAME9_TIME) {
        NVIC_DisableIRQ(SysTicK_IRQn);
        update_buffer(frame9);
        NVIC_EnableIRQ(SysTicK_IRQn);
    }
    if (animation_frame_counter==FRAME8_TIME) {
        NVIC_DisableIRQ(SysTicK_IRQn);
        update_buffer(frame8);
        NVIC_EnableIRQ(SysTicK_IRQn);
    }
    if (animation_frame_counter==FRAME7_TIME) {
        NVIC_DisableIRQ(SysTicK_IRQn);
        update_buffer(frame7);
        NVIC_EnableIRQ(SysTicK_IRQn);
    }
    if (animation_frame_counter==FRAME6_TIME) {
        NVIC_DisableIRQ(SysTicK_IRQn);
        update_buffer(frame6);
        NVIC_EnableIRQ(SysTicK_IRQn);
    }
    if (animation_frame_counter==FRAME5_TIME) {
        NVIC_DisableIRQ(SysTicK_IRQn);
        update_buffer(frame5);
        NVIC_EnableIRQ(SysTicK_IRQn);
    }
    if (animation_frame_counter==FRAME4_TIME) {
        NVIC_DisableIRQ(SysTicK_IRQn);
        update_buffer(frame4);
        NVIC_EnableIRQ(SysTicK_IRQn);
    }
    if (animation_frame_counter==FRAME3_TIME) {
        NVIC_DisableIRQ(SysTicK_IRQn);
        update_buffer(frame3);
        NVIC_EnableIRQ(SysTicK_IRQn);
    }
    if (animation_frame_counter==FRAME2_TIME) {
        NVIC_DisableIRQ(SysTicK_IRQn);
        update_buffer(frame2);
        NVIC_EnableIRQ(SysTicK_IRQn);
    }
    if (animation_frame_counter==FRAME1_TIME) {
        NVIC_DisableIRQ(SysTicK_IRQn);
        update_buffer(frame1);
        NVIC_EnableIRQ(SysTicK_IRQn);
    }
    if (animation_frame_counter==FRAME0_TIME) {
        NVIC_DisableIRQ(SysTicK_IRQn);
        update_buffer(frame0);
        NVIC_EnableIRQ(SysTicK_IRQn);
    }
}

#endif /* USER_CHARLIEPLEXING_H_ */
