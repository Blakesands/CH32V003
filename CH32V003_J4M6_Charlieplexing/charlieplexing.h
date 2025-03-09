/* charlieplexing.h Created on: 04/03/25 Author: BK */

#ifndef USER_CHARLIEPLEXING_H_
#define USER_CHARLIEPLEXING_H_

#include <stdbool.h>
#include <string.h>
#include "debug.h" // includes core files

void GPIO_Config(void);
void start_systick (void);
void SysTick_Handler(void);
void update_buffer(const bool src[], uint16_t offset);
void set_pin_mode(uint8_t, int8_t);
void update_led_pins(void);
void animate_frames (void);

// unused
void clear_led_buffer(void);
void all_leds_on(void);
void every_x_leds_on(uint8_t);

#endif /* USER_CHARLIEPLEXING_H_ */
