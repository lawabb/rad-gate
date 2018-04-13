/*****************************************
 *  RemoteArDuino (RAD) Gate
 *  (c) Danny Frencham 2017
 *****************************************/

#include <stdint.h>

#ifndef _CONSTANTS_H
#define _CONSTANTS_H

#define VERSION "0.8.6"

// Light tree 
#define LIGHT_TREE_RELAY_ENABLE 1 // set to 1 for RELAY operation
#if LIGHT_TREE_RELAY_ENABLE
#define LIGHT_TREE_RELAY_ON 1  // LIGHT_TREE_RELAYs output condition for LED ON
#endif
#define LIGHT_TREE_STRIP 1     // set to 1 for Strip type LEDS eg Neopixel
#if LIGHT_TREE_STRIP
#define NUM_LEDS    8        // how many LEDs do you have?
#define BRIGHTNESS  10       // 255 is max bright
#define LED_TYPE    WS2812   // WS2812B for genuine NeoPixel
#define COLOR_ORDER GRB      // RGB for genuine NeoPixel
#endif

// sound player
// uncomment if using Adafruit sound fx boards
//#define HARDWARE_SOUNDBOARD_ADAFRUIT
// or uncomment "JQ6500" if using the cheap and friendly Aliexpress JQ6500
#define HARDWARE_SOUNDBOARD_JQ6500
#define SFX_VOLUME 30               // 30 is max on JQ6500

// Delay times in msec as per UCI cycling regulations
// Should not need changing unless regulations change
#define DELAY_INTERVAL_MS 60
#define DELAY_DROP_TONE_MS 60
#define DELAY_DROP_TONE_FINAL_MS 2250
#define DELAY_GATE_RISE_WARN_MS 25
#define DELAY_ABORT_TONE_1_MS 220
#define DELAY_ABORT_TONE_2_MS 440
#define DELAY_RAND_MIN 100  // random start min
#define DELAY_RAND_MAX 2700 // random start max

// Tone frequencies as per UCI cycling regulations (in Hz)
// Should not need changing unless regulations change
#define TONE_DROP_HZ 632
#define TONE_GATE_RISE 1150
#define TONE_ABORT_1_HZ 740
#define TONE_ABORT_2_HZ 680

// Pin assignments for UNO
#define PIN_SFX_RST 0            // Adafruit sound board reset
#define PIN_SFX_ACT 1            // Adafruit sound board active
#define PIN_BUTTON_GO 2          // The GO/ABORT button;  interrupt0
#define PIN_SENSOR 3             // light beam switch/sensor; interrupt1
#define PIN_LED_ACTIVE 4         //  GO button is activated LED
#define PIN_DISPLAY_DATA 5       // 4 digit seven seg LED display -> DS
#define PIN_DISPLAY_LATCH 6      // 4 digit seven seg LED display -> ST_CP
#define PIN_DISPLAY_CLOCK 7      // 4 digit seven seg LED display -> SH_CP
#define PIN_RELAY 8              // relay, or MOSFET for GATE
#define PIN_SFX_RX 9             // sound board receive (Arduino Tx) 
#define PIN_SFX_TX 10            // sound board transmit (Arduino Rx)
#define PIN_SPEAKER 11           // tone output
//#define  12      
#define PIN_LED 13               // light tree serial data
#define PIN_LIGHT_TREE_RELAY_1 14  // Red LED
#define PIN_LIGHT_TREE_RELAY_2 15  // Orange LED
#define PIN_LIGHT_TREE_RELAY_3 16  // Orange LED 
#define PIN_LIGHT_TREE_RELAY_4 17  // Green LED
#define PIN_BAT_MON 18           // Analogue Input to measure battery voltage
#define PIN_ANA_VIN 19           // For random seed for random delay

// Gate defines
#define SHORT_PULSE_GATE 1   // 1 for short pulse gate activation. Typically when using solenoid
#define GATE_ACTIVE_MS 250   // time for solenoid to operate. Only applies if SHORT_GATE_PULSE = 1
#define GATE_ACTIVE_LEVEL 1  // Level of ouput pin that causes gate drop. 1 = HIGH = 5V
    
#define ENABLE_VOICE_CADENCE 1  // Enable voice announcements

// Sound file_content to file_number on JQ6500 mapping
// Files 1 and 2 Should be created per UCI cycling regulations
#define SFX_PREP 1
#define SFX_WATCH_GATE 2
#define SFX_ZERO 3
#define SFX_ONE 4 
#define SFX_TWO 5  
#define SFX_THREE 6
#define SFX_FOUR 7
#define SFX_FIVE 8
#define SFX_SIX 9
#define SFX_SEVEN 10
#define SFX_EIGHT 11
#define SFX_NINE 12
#define SFX_POINT 13
#define SFX_SECONDS 14
#define SFX_LOW_BAT_ALM 15

#define BAT_ALARM_VOLTS 10500    // milliVolts

//struct sfx_maps;
typedef struct {
   char SFX_CHAR;
   int  SFX_MESG;
} sfx_map;

// variables
extern volatile uint32_t GATE_DROP_START;
extern bool FLAG_GATE_DOWN;

#endif
