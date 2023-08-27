// Send the colour data for the entire strip of LEDs
// TODO add brightness control
void SHOWTIME(uint8_t BUFFER_LEDS[num_leds][3])
{
    // send LED info
    for (int i = 0; i < num_leds; i++) {
        LED_SendColour(BUFFER_LEDS[i][0], BUFFER_LEDS[i][1], BUFFER_LEDS[i][2]);
    }
    // Delay for minimum 50uS between pinwiggles to signal next frame to WS2812B LEDS
    Delay_Us(50);
}


void LED_OFF(void)
{
    for (int i = 0; i <  256; i++) {
        LED_SendColour(0, 0, 0);
    }
    Delay_Us(50);
}

void LED_RED(uint8_t brightness)
{
    for (int i = 0; i < num_leds; i++) {
        LED_SendColour(brightness, 0, 0);
    }
    Delay_Us(50);
    __WFI(); // uncomment to program static colour and go to sleep
}

void LED_GREEN(uint8_t brightness)
{
    for (int i = 0; i < num_leds; i++) {
        LED_SendColour(0, brightness, 0);
    }
    Delay_Us(50);
    __WFI(); // uncomment to program static colour and go to sleep
}

void LED_BLUE(uint8_t brightness)
{
    for (int i = 0; i < num_leds; i++) {
        LED_SendColour(0, 0, brightness);
    }
    Delay_Us(50);
    __WFI(); // uncomment to program static colour and go to sleep
}

void LED_WARM_WHITE(void)
{
    for (int i = 0; i < num_leds; i++) {
        LED_SendColour(255, 100, 50);
    }
    Delay_Ms(150);
    __WFI(); // uncomment to program static colour and go to sleep
}

void LED_COLD_WHITE(void)
{
    for (int i = 0; i < num_leds; i++) {
        LED_SendColour(255, 180, 235);
    }
    Delay_Ms(150);
    __WFI(); // uncomment to program static colour and go to sleep
}

void LED_CUSTOM(uint16_t red, uint16_t green, uint16_t blue)
{
    for (int i = 0; i < num_leds; i++) {
        LED_SendColour(red, green, blue);
    }
    Delay_Us(150);
    __WFI(); // uncomment to program static colour and go to sleep
}

void LED_RGB_FLASH(uint16_t speed, uint8_t brightness)
{
    for (int i = 0; i < num_leds; i++) {
    LED_SendColour(brightness, 0, 0); }
    Delay_Ms(speed);
    for (int i = 0; i < num_leds; i++) {
    LED_SendColour(0, brightness, 0); }
    Delay_Ms(speed);
    for (int i = 0; i < num_leds; i++) {
    LED_SendColour(0, 0, brightness);}
    Delay_Ms(speed);

}

void LED_STATIC_RANDOM (void){ // fills BUFFER_LEDS with random values seeded from ADC reading a floating pin A4
GPIO_InitTypeDef GPIO_InitStructure = {0};
ADC_InitTypeDef ADC_InitStructure = {0};

RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
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
ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_15Cycles);
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

for (int i = 0; i < num_leds; ++i) {

        BUFFER_LEDS[i][0] = rand_lookup_2[(rand() % 255)]; // % modulus function
        BUFFER_LEDS[i][1] = rand_lookup_2[(rand() % 255)];
        BUFFER_LEDS[i][2] = rand_lookup_2[(rand() % 255)];
    }

SHOWTIME(BUFFER_LEDS);

__WFI(); // uncomment to program static colour and go to sleep
}

// used by RAINBOWS
uint8_t * Wheel(uint8_t WheelPos) {
  static uint8_t c[3];

  if(WheelPos < 85) {
   c[0]=WheelPos * 3;
   c[1]=255 - WheelPos * 3;
   c[2]=0;
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   c[0]=255 - WheelPos * 3;
   c[1]=0;
   c[2]=WheelPos * 3;
  } else {
   WheelPos -= 170;
   c[0]=0;
   c[1]=WheelPos * 3;
   c[2]=255 - WheelPos * 3;
  }
  return c;
}
void LED_RAINBOWS(uint16_t speed, uint16_t width) {
  uint8_t *c;
  uint16_t i, j;

  for(j=0; j<256; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< width; i++) {
      c=Wheel(((i * 256 / width) + j) & 255);
      BUFFER_LEDS[i][0] = *c ;
      BUFFER_LEDS[i][1] = *(c+1) ;
      BUFFER_LEDS[i][2] = *(c+2) ;
    }
    SHOWTIME(BUFFER_LEDS);
   Delay_Ms(speed);
  }
}

void LED_BLUEFIRE(uint16_t speed)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    ADC_InitTypeDef ADC_InitStructure = {0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
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
    ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_15Cycles);
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
    //    LED_OFF();
    uint8_t LEDS[num_leds][3] = {};
    //    srand((unsigned) ADC_val);
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
void LED_WHITE_FADE(uint16_t speed)
{
// Increment count for sine wave animation
    if (flag==0) {
        count++;
    }
    if (flag==1) {
        count--;
        }
    if (count >= 85) {
        count = 85;
    flag = 1;
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

void LED_WHITE_FLASH (uint16_t max, uint16_t min)
    {
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


