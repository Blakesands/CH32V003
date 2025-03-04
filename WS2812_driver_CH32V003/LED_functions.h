// Initialise the variables for animation
double wavePos = 0;
static uint8_t c[3]; // for rainbows
uint8_t flag = 0;
uint32_t count = 1;

void LED_RAINBOWS(uint16_t speed, uint16_t width);
uint8_t * Wheel(uint8_t WheelPos);

void LED_OFF(void){
    //increase number if string is longer than 256
    for (int i = 0; i <  256; i++) { 
        LED_SendColour(0, 0, 0, 1);
    }
    Delay_Us(50);
}

void LED_RED(uint8_t brightness){
    for (int i = 0; i < num_leds; i++) {
        LED_SendColour(brightness, 0, 0, BRIGHT);
    }
    Delay_Us(50);
//    __WFI(); // uncomment to program static colour and go to sleep
}

void LED_GREEN(uint8_t brightness){
    for (int i = 0; i < num_leds; i++) {
        LED_SendColour(0, brightness, 0, BRIGHT);
    }
    Delay_Us(50);
    __WFI(); // uncomment to program static colour and go to sleep
}

void LED_BLUE(uint8_t brightness){
    for (int i = 0; i < num_leds; i++) {
        LED_SendColour(0, 0, brightness, BRIGHT);
    }
    Delay_Us(50);
    __WFI(); // uncomment to program static colour and go to sleep
}

void LED_WARM_WHITE(void){
    for (int i = 0; i < num_leds; i++) {
        LED_SendColour(255, 100, 50, BRIGHT);
    }
    Delay_Ms(150);
    __WFI(); // uncomment to program static colour and go to sleep
}

void LED_COLD_WHITE(void){
    for (int i = 0; i < num_leds; i++) {
        LED_SendColour(255, 180, 235, BRIGHT);
    }
    Delay_Ms(150);
    __WFI(); // uncomment to program static colour and go to sleep
}

void LED_CUSTOM(uint16_t red, uint16_t green, uint16_t blue){
    for (int i = 0; i < num_leds; i++) {
        LED_SendColour(red, green, blue, BRIGHT);
    }
    Delay_Us(150);
    __WFI(); // uncomment to program static colour and go to sleep
}

void LED_RGB_FLASH(uint16_t speed, uint8_t brightness){
    for (int i = 0; i < num_leds; i++) {
    LED_SendColour(brightness, 0, 0, BRIGHT); }
    Delay_Ms(speed);
    for (int i = 0; i < num_leds; i++) {
    LED_SendColour(0, brightness, 0, BRIGHT); }
    Delay_Ms(speed);
    for (int i = 0; i < num_leds; i++) {
    LED_SendColour(0, 0, brightness, BRIGHT);}
    Delay_Ms(speed);
}

// fills BUFFER_LEDS with random values seeded from ADC reading a floating pin A4
void LED_STATIC_RANDOM (void){ 
    int COL [3];
    COL [1] = rand_lookup_2[(rand() % 128)];
    COL [2] = rand_lookup_2[(rand() % 128)];
    COL [3]= rand_lookup_2[(rand() % 128)];
    
    if (COL [1] + COL[2] + COL[3] == 765){
        while (1){
            LED_RAINBOWS(50, 5);
        }
    }
    
    //if (COL [1] + COL[2] + COL[3] == 189)
    //{
    //    while (1){
    //        LED_RAINBOWS(30, 2);
    //    }
    //}
    
    if (COL [1] + COL[2] + COL[3] == 0){
        COL[1] = 220;
        COL[2] = 0;
        COL[3] = 0;
    }
    for (int i = 0; i < num_leds; ++i) {
        BUFFER_LEDS[i][0] = COL[1];
        BUFFER_LEDS[i][1] = COL[2];
        BUFFER_LEDS[i][2] = COL[3];
    }
    
    SHOWTIME(BUFFER_LEDS);
    __WFI(); // uncomment to program static colour and go to sleep
}

// used by RAINBOWS
uint8_t * Wheel(uint8_t WheelPos){
  if(WheelPos < 85) {
   c[2]=WheelPos * 3;
   c[1]=255 - WheelPos * 3;
   c[0]=0;
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   c[2]=255 - WheelPos * 3;
   c[1]=0;
   c[0]=WheelPos * 3;
  } else {
   WheelPos -= 170;
   c[2]=0;
   c[1]=WheelPos * 3;
   c[0]=255 - WheelPos * 3;
  }
  return c;
}

void LED_RAINBOWS(uint16_t speed, uint16_t width) {
  uint8_t *c;
  uint16_t i, j;
  for(j=0; j<256; j++) {
        for(i=0; i< num_leds; i++) {
          c=Wheel(((i * 255 / width -j)) & 255);
          BUFFER_LEDS[i][0] = (*c);
          BUFFER_LEDS[i][1] = (*(c+1)) ;
          BUFFER_LEDS[i][2] = (*(c+2)) ;
        }
        SHOWTIME(BUFFER_LEDS);
        Delay_Ms(speed);
  }
}

void LED_RAINBOWS_WIDTH(uint16_t speed) { //uint16_t width
  uint8_t *c;
  uint16_t i, j;
  uint8_t sin_val;
  for(j=0; j<256; j++) {

  sin_val = sine_lookup[j]; // Adjust the multiplier as needed for the desired frequency and amplitude
  uint16_t width = Map_Range(sin_val, 0, 256, num_leds/4, num_leds*3);
    for(i=0; i< num_leds; i++) {
      c=Wheel(((i * 256 / width -j)) & 255);
      BUFFER_LEDS[i][0] = *c;
      BUFFER_LEDS[i][1] = *(c+1) ;
      BUFFER_LEDS[i][2] = *(c+2) ;
    }
    SHOWTIME(BUFFER_LEDS);
   Delay_Ms(speed);
  }
}

void LED_BLUEFIRE(uint16_t speed){
    uint8_t LEDS[num_leds][3] = {};
    // Set the first LED to a random colour
    BUFFER_LEDS[0][0] = sine_lookup[(rand() % 85)];
    BUFFER_LEDS[0][1] = sine_lookup[(rand() % 2)];
    BUFFER_LEDS[0][2] = sine_lookup[(rand() % 10)];

    if (!(BUFFER_LEDS[0][2]&&BUFFER_LEDS[0][0])) { ///Yay! for logical operators
        BUFFER_LEDS[0][0] = 85;
        BUFFER_LEDS[0][1] = 0;
        BUFFER_LEDS[0][2] = 0;
    }
    if ((BUFFER_LEDS[0][2]) >8) {
        BUFFER_LEDS[0][0] = 0;
        BUFFER_LEDS[0][1] = 0;
        BUFFER_LEDS[0][2] = 85;
    }
    SHOWTIME(BUFFER_LEDS);
    // Shift the colours of the LEDs by one position
    for (int i = num_leds - 1; i > 0; i--) {
        LEDS[i][0] = BUFFER_LEDS[i - 1][0];
        LEDS[i][1] = BUFFER_LEDS[i - 1][1];
        LEDS[i][2] = BUFFER_LEDS[i - 1][2];
        BUFFER_LEDS[i][0] = ((LEDS[i][0]));
        BUFFER_LEDS[i][1] = ((LEDS[i][1]));
        BUFFER_LEDS[i][2] = ((LEDS[i][2]));
    }
    SHOWTIME(BUFFER_LEDS);
    Delay_Ms(speed+(rand() % (speed*5)) );
}

// This function applies the sine wave to the LED colours and updates the buffer
void LED_WHITE_FADE(uint16_t speed){
// Increment count for sine wave animation
    if (!flag) {
        count++;
    }
    if (flag) {
        count--;
        }
    if (count >= 85) {
        count = 85;
    f    lag = 1;
    }
    if (count <= 10) {
        count = 10;
        flag = 0;
    }
    // Apply the sine wave to the LED colors
    for (int i = 0; i < num_leds; i++) {
        // Update the LED colour values with the mapped sine wave value
        BUFFER_LEDS[i][0] = sine_lookup_2[count]; // set red value to sine wave value
        BUFFER_LEDS[i][1] = sine_lookup_2[count]; // set green value to sine wave value
        BUFFER_LEDS[i][2] = sine_lookup_2[count]; // set blue value to sine wave value
    }
    SHOWTIME(BUFFER_LEDS);
    Delay_Ms(speed);
}

void LED_WHITE_FLASH (uint16_t max, uint16_t min){
    BUFFER_LEDS[0][0] = 200;
    BUFFER_LEDS[0][1] = 200;
    BUFFER_LEDS[0][2] = 200;
    BUFFER_LEDS[1][0] = 0;
    BUFFER_LEDS[1][1] = 0;
    BUFFER_LEDS[1][2] = 0;

    // Show the updated buffer on the LED strip
    SHOWTIME(BUFFER_LEDS);
    Delay_Ms(count);
    BUFFER_LEDS[0][0] = 0;
    BUFFER_LEDS[0][1] = 0;
    BUFFER_LEDS[0][2] = 0;
    BUFFER_LEDS[1][0] = 200;
    BUFFER_LEDS[1][1] = 200;
    BUFFER_LEDS[1][2] = 200;

    SHOWTIME(BUFFER_LEDS);
    Delay_Ms(count);

    if (flag) {
        count++;
        if (count > max) {
        flag=0;
        }
    }
    else {
        count--;
        if (count < min) {
        flag=1;
        }
    }
}


