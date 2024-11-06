// Send a single bit, Check mark/space ratio of the data with an oscilloscope, adjust nops

void LED_SendBit(uint8_t bit)    //// Send a 1 bit to WS2812B LED
// They want 24 bits per led in the string with a 50uS gap to indicate end of frame
{
    if (bit)
        {
        GPIOD->BSHR = 1 << 6; // put pin D6 high and wait for 800nS
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
        GPIOD->BCR = 1 << 6; // put pin D6 low and exit, 400nS is taken up by other functions
        return;
        }
        GPIOD->BSHR = 1 << 6; // put pin D6 high and wait for 400nS
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");
        GPIOD->BCR = 1 << 6; // put pin D6 low and wait for 400nS, 200nS is taken up by other functions
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
}

// Used for global brightness control
int Map_Range(int value, int inMin, int inMax, int outMin, int outMax) {
    // Make sure the input value is within the input range
    value = value < inMin ? inMin : value;
    value = value > inMax ? inMax : value;
    // Map the value from the input range to the output range
    return (value - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}

// Send a single RGB colour for a single WS2812B LED
void LED_SendColour(uint8_t Red, uint8_t Green, uint8_t Blue, uint8_t brightness)
{
    uint8_t green = Map_Range(Green, 0, 255, 0, brightness);
    uint8_t red = Map_Range(Red, 0, 255, 0, brightness);
    uint8_t blue = Map_Range(Blue, 0, 255, 0, brightness);

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
//    Delay_Us(50); // signals end of transmission
}
