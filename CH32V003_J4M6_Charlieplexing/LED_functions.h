//* animations.h Created on: 04/03/25 Author: BK */

#ifndef USER_LED_FUNCTIONS_H_
#define USER_LED_FUNCTIONS_H_

// Clear the LED buffer (turn all LEDs off)
void clear_led_buffer(void) {
    memset(led_buffer, 0, sizeof(led_buffer));
}

void leds_on(int every)
{
    clear_led_buffer();
    for (int i = 0; i < NUM_LEDS; i++){
        if (i%every==0) {
            led_buffer[i] = 1;
        }
    }
}

#endif /* USER_LED_FUNCTIONS_H_ */
