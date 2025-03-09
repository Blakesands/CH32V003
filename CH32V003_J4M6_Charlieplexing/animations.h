//* animation.h Created on: 04/03/25 Author: BK */

#ifndef USER_ANIMATIONS_H_
#define USER_ANIMATIONS_H_

// animation frames
#define NUMBER_FRAMES 10

#define NUM_PINS 6
#define NUM_LEDS 30

// Define the animation and it's loop time
#define ANIMATION line // line arrow heart tunnel eye
#define LOOP_TIME 3000 // 10k is approx 1 second, 40k for heart, 4k for line/arrow


//#define CUSTOM_TIMING
#define AUTO_TIMING

// Define custom frame timings (for heart in this case)
#ifdef CUSTOM_TIMING
#define FRAME0_TIME 0
#define FRAME1_TIME 10000
#define FRAME2_TIME 11000
#define FRAME3_TIME 12000
#define FRAME4_TIME 13000
#define FRAME5_TIME 14000
#define FRAME6_TIME 15000
#define FRAME7_TIME 16000
#define FRAME8_TIME 18000
#define FRAME9_TIME 30000
#endif

// Or set to auto frame rate (for line, arrow)
#ifdef AUTO_TIMING
#define FRAME0_TIME 0
#define FRAME1_TIME (LOOP_TIME/NUMBER_FRAMES)
#define FRAME2_TIME FRAME1_TIME*2
#define FRAME3_TIME FRAME1_TIME*3
#define FRAME4_TIME FRAME1_TIME*4
#define FRAME5_TIME FRAME1_TIME*5
#define FRAME6_TIME FRAME1_TIME*6
#define FRAME7_TIME FRAME1_TIME*7
#define FRAME8_TIME FRAME1_TIME*8
#define FRAME9_TIME FRAME1_TIME*9
#endif

bool heart[NUM_LEDS*NUMBER_FRAMES] ={

                        0,0,0,0,0,
                        0,1,1,1,0,
                        0,0,1,0,0,
                        0,0,1,0,0,
                        0,1,1,1,0,
                        0,0,0,0,0,

                        0,0,0,0,0,
                        0,0,0,0,0,
                        0,0,1,0,0,
                        0,0,0,0,0,
                        0,0,0,0,0,
                        0,0,0,0,0,

                        0,0,0,0,0,
                        0,0,0,0,0,
                        0,1,1,1,0,
                        0,0,0,0,0,
                        0,0,0,0,0,
                        0,0,0,0,0,

                        0,0,0,0,0,
                        0,1,1,1,0,
                        0,1,1,1,0,
                        0,1,1,1,0,
                        0,0,0,0,0,
                        0,0,0,0,0,

                        0,0,0,0,0,
                        0,1,1,1,0,
                        1,1,1,1,1,
                        0,1,1,1,0,
                        0,0,0,0,0,
                        0,0,0,0,0,

                        0,0,0,0,0,
                        0,1,1,1,0,
                        1,1,1,1,1,
                        0,1,1,1,0,
                        0,1,1,1,0,
                        0,0,0,0,0,

                        0,0,0,0,0,
                        0,1,1,1,0,
                        1,1,1,1,1,
                        1,1,1,1,1,
                        0,1,1,1,0,
                        0,0,0,0,0,

                        0,0,0,0,0,
                        1,1,1,1,1,
                        1,1,1,1,1,
                        1,1,1,1,1,
                        0,1,1,1,0,
                        0,0,0,0,0,

                        0,1,0,1,0,
                        1,1,1,1,1,
                        1,1,1,1,1,
                        1,1,1,1,1,
                        0,1,1,1,0,
                        0,0,1,0,0,

                        0,0,0,0,0,
                        0,1,0,1,0,
                        0,1,0,1,0,
                        0,1,0,1,0,
                        0,1,1,1,0,
                        0,0,0,0,0
};

/// rotating line animation

bool line[NUM_LEDS*NUMBER_FRAMES] ={

                            0,1,1,0,0,
                            0,0,1,0,0,
                            0,0,1,0,0,
                            0,0,0,0,0,
                            0,0,0,0,0,
                            0,0,0,0,0,

                            0,0,0,1,1,
                            0,0,0,1,0,
                            0,0,1,0,0,
                            0,0,0,0,0,
                            0,0,0,0,0,
                            0,0,0,0,0,

                            0,0,0,0,0,
                            0,0,0,0,1,
                            0,0,1,1,1,
                            0,0,0,0,0,
                            0,0,0,0,0,
                            0,0,0,0,0,

                            0,0,0,0,0,
                            0,0,0,0,0,
                            0,0,0,0,1,
                            0,0,1,1,1,
                            0,0,0,0,0,
                            0,0,0,0,0,

                            0,0,0,0,0,
                            0,0,0,0,0,
                            0,0,0,0,0,
                            0,0,1,0,0,
                            0,0,0,1,1,
                            0,0,0,0,1,

                            0,0,0,0,0,
                            0,0,0,0,0,
                            0,0,0,0,0,
                            0,0,1,0,0,
                            0,0,1,0,0,
                            0,0,1,1,0,

                            0,0,0,0,0,
                            0,0,0,0,0,
                            0,0,0,0,0,
                            0,0,1,0,0,
                            0,1,0,0,0,
                            1,1,0,0,0,

                            0,0,0,0,0,
                            0,0,0,0,0,
                            0,0,0,0,0,
                            1,1,1,0,0,
                            1,0,0,0,0,
                            0,0,0,0,0,

                            0,0,0,0,0,
                            0,0,0,0,0,
                            1,1,1,0,0,
                            1,0,0,0,0,
                            0,0,0,0,0,
                            0,0,0,0,0,

                            1,0,0,0,0,
                            1,1,0,0,0,
                            0,0,1,0,0,
                            0,0,0,0,0,
                            0,0,0,0,0,
                            0,0,0,0,0
};

/// arrow animation

bool arrow[NUM_LEDS*NUMBER_FRAMES] ={

                            0,0,0,0,0,
                            0,0,0,0,0,
                            0,0,0,0,0,
                            0,0,0,0,0,
                            0,0,0,0,0,
                            0,0,1,0,0,

                            0,0,0,0,0,
                            0,0,0,0,0,
                            0,0,0,0,0,
                            0,0,0,0,0,
                            0,0,1,0,0,
                            0,1,1,1,0,

                            0,0,0,0,0,
                            0,0,0,0,0,
                            0,0,0,0,0,
                            0,0,1,0,0,
                            0,1,1,1,0,
                            1,1,1,1,1,

                            0,0,0,0,0,
                            0,0,0,0,0,
                            0,0,1,0,0,
                            0,1,1,1,0,
                            1,1,1,1,1,
                            0,0,0,0,0,

                            0,0,0,0,0,
                            0,0,1,0,0,
                            0,1,1,1,0,
                            1,1,1,1,1,
                            0,0,0,0,0,
                            0,0,0,0,0,

                            0,0,1,0,0,
                            0,1,1,1,0,
                            1,1,1,1,1,
                            0,0,0,0,0,
                            0,0,0,0,0,
                            0,0,0,0,0,

                            0,1,1,1,0,
                            1,1,1,1,1,
                            0,0,0,0,0,
                            0,0,0,0,0,
                            0,0,0,0,0,
                            0,0,0,0,0,

                            1,1,1,1,1,
                            0,0,0,0,0,
                            0,0,0,0,0,
                            0,0,0,0,0,
                            0,0,0,0,0,
                            0,0,0,0,0,

                            0,0,0,0,0,
                            0,0,0,0,0,
                            0,0,0,0,0,
                            0,0,0,0,0,
                            0,0,0,0,0,
                            0,0,0,0,0,

                            0,0,0,0,0,
                            0,0,0,0,0,
                            0,0,0,0,0,
                            0,0,0,0,0,
                            0,0,0,0,0,
                            0,0,0,0,0
};

bool tunnel[NUM_LEDS*NUMBER_FRAMES] ={

                            0,0,0,0,0,
                            0,0,0,0,0,
                            0,0,0,0,0,
                            0,0,0,0,0,
                            0,0,0,0,0,
                            0,0,0,0,0,

                            0,0,0,0,0,
                            0,0,0,0,0,
                            0,0,1,0,0,
                            0,0,1,0,0,
                            0,0,0,0,0,
                            0,0,0,0,0,

                            0,0,0,0,0,
                            0,0,1,0,0,
                            0,1,0,1,0,
                            0,1,0,1,0,
                            0,0,1,0,0,
                            0,0,0,0,0,

                            0,0,0,0,0,
                            0,1,1,1,0,
                            0,1,0,1,0,
                            0,1,0,1,0,
                            0,1,1,1,0,
                            0,0,0,0,0,

                            1,1,1,1,1,
                            1,0,0,0,1,
                            1,0,0,0,1,
                            1,0,0,0,1,
                            1,0,0,0,1,
                            1,1,1,1,1,

                            0,0,0,0,0,
                            0,0,0,0,0,
                            0,0,0,0,0,
                            0,0,0,0,0,
                            0,0,0,0,0,
                            0,0,0,0,0,

                            0,0,0,0,0,
                            0,0,0,0,0,
                            0,0,1,0,0,
                            0,0,1,0,0,
                            0,0,0,0,0,
                            0,0,0,0,0,

                            0,0,0,0,0,
                            0,0,1,0,0,
                            0,1,0,1,0,
                            0,1,0,1,0,
                            0,0,1,0,0,
                            0,0,0,0,0,

                            0,0,0,0,0,
                            0,1,1,1,0,
                            0,1,0,1,0,
                            0,1,0,1,0,
                            0,1,1,1,0,
                            0,0,0,0,0,

                            1,1,1,1,1,
                            1,0,0,0,1,
                            1,0,0,0,1,
                            1,0,0,0,1,
                            1,0,0,0,1,
                            1,1,1,1,1


};

bool eye[NUM_LEDS*NUMBER_FRAMES] ={

                            0,1,1,1,0,
                            1,0,0,0,1,
                            1,0,1,0,1,
                            1,0,0,0,1,
                            0,1,1,1,0,
                            0,0,0,0,0,

                            0,1,1,1,0,
                            1,0,0,0,1,
                            1,0,1,0,1,
                            1,0,0,0,1,
                            0,1,1,1,0,
                            0,0,0,0,0,

                            0,0,0,0,0,
                            1,1,1,1,1,
                            1,0,1,0,1,
                            1,1,1,1,1,
                            0,0,0,0,0,
                            0,0,0,0,0,

                            0,0,0,0,0,
                            0,0,0,0,0,
                            1,1,1,1,1,
                            0,0,0,0,0,
                            0,0,0,0,0,
                            0,0,0,0,0,

                            0,0,0,0,0,
                            0,0,0,0,0,
                            0,1,1,1,0,
                            0,0,0,0,0,
                            0,0,0,0,0,
                            0,0,0,0,0,

                            0,0,0,0,0,
                            0,0,0,0,0,
                            1,1,1,1,1,
                            0,0,0,0,0,
                            0,0,0,0,0,
                            0,0,0,0,0,

                            0,0,0,0,0,
                            1,1,1,1,1,
                            1,0,1,0,1,
                            1,1,1,1,1,
                            0,0,0,0,0,
                            0,0,0,0,0,

                            0,1,1,1,0,
                            1,0,0,0,1,
                            1,0,1,0,1,
                            1,0,0,0,1,
                            0,1,1,1,0,
                            0,0,0,0,0,

                            0,1,1,1,0,
                            1,0,0,0,1,
                            1,0,1,0,1,
                            1,0,0,0,1,
                            0,1,1,1,0,
                            0,0,0,0,0,

                            0,1,1,1,0,
                            1,0,0,0,1,
                            1,0,1,0,1,
                            1,0,0,0,1,
                            0,1,1,1,0,
                            0,0,0,0,0
};

#endif /* USER_ANIMATIONS_H_ */

// // animations.h (Partial Example)
// #ifndef ANIMATIONS_H
// #define ANIMATIONS_H

// #include <stdbool.h>
// #include <stdint.h>

// // ... (NUM_PINS, NUM_LEDS, NUMBER_FRAMES, etc.)

// typedef struct {
//     const bool *frames;
//     uint8_t num_frames;
//     uint8_t num_leds;
//     uint32_t *frame_times; 
//     uint32_t default_frame_time;
//     const char *name;
//     uint8_t current_frame;
//     uint32_t frame_counter;
// } Animation_t;

// extern const bool arrow_frames[];
// extern const uint32_t arrow_times[];
// extern const Animation_t arrow_animation;

// extern const bool heart_frames[];
// extern const uint32_t heart_times[];
// extern const Animation_t heart_animation;

// // ... (other animation declarations)

// // define the animations to be used
// #define ANIMATION arrow_animation

// #endif // ANIMATIONS_H

// //animations.c
// #include "animations.h"

// const bool arrow_frames[NUM_LEDS*NUMBER_FRAMES] = { /* ... */ };
// const uint32_t arrow_times[]={/*...*/}
// const Animation_t arrow_animation = {
//   arrow_frames,
//   NUMBER_FRAMES, // NUMBER_FRAMES
//   NUM_LEDS,
//   arrow_times,
//   (LOOP_TIME/NUMBER_FRAMES), // default frame rate
//   "Arrow",
//   0,
//   0
// };

// const bool heart_frames[NUM_LEDS*NUMBER_FRAMES] = { /* ... */ };
// const uint32_t heart_times[]={/*...*/}
// const Animation_t heart_animation = {
//   heart_frames,
//   NUMBER_FRAMES, // NUMBER_FRAMES
//   NUM_LEDS,
//   heart_times,
//   1000, // default frame rate
//   "Heart",
//   0,
//   0
// };


// //charlieplexing.c (update frames)
// void animate_frames (void){
//     const Animation_t *current_animation = &ANIMATION;
//     if (current_animation->frame_counter >= current_animation->default_frame_time) {
//         current_animation->frame_counter=0;
//         current_animation->current_frame++;
//         if(current_animation->current_frame>=current_animation->num_frames){
//             current_animation->current_frame=0;
//         }
//         update_buffer(current_animation->frames, current_animation->current_frame*current_animation->num_leds);
//     } else {
//         current_animation->frame_counter++;
//     }
// }
