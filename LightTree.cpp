/*****************************************
 *  RemoteArDuino (RAD) Gate
 *  (c) Danny Frencham 2017
 *****************************************/

#include "constants.h"
#include "LightTree.h"
#include <FastLED.h>

#if LIGHT_TREE_STRIP
CRGB leds[NUM_LEDS];
int team_size = NUM_LEDS/4;  //  no of LEDS in each color
#endif

LightTree::LightTree() { }

void LightTree::initialise() { 
  #if (LIGHT_TREE_STRIP)
    serial_print("Init Light Strip");
    delay(2000); // power-up safety delay
    FastLED.addLeds<LED_TYPE, PIN_LED, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.show();
    FastLED.setBrightness(BRIGHTNESS); 
  #endif
  #if (LIGHT_TREE_RELAY_ENABLE)
    serial_print("Init Light Relays");
    pinMode(PIN_LIGHT_TREE_RELAY_1, OUTPUT);
    pinMode(PIN_LIGHT_TREE_RELAY_2, OUTPUT);
    pinMode(PIN_LIGHT_TREE_RELAY_3, OUTPUT);
    pinMode(PIN_LIGHT_TREE_RELAY_4, OUTPUT);
  #endif 
}

void LightTree::led_reset() {
  #if (LIGHT_TREE_RELAY_ENABLE)
    digitalWrite(PIN_LIGHT_TREE_RELAY_1, !LIGHT_TREE_RELAY_ON);
    digitalWrite(PIN_LIGHT_TREE_RELAY_2, !LIGHT_TREE_RELAY_ON);
    digitalWrite(PIN_LIGHT_TREE_RELAY_3, !LIGHT_TREE_RELAY_ON);
    digitalWrite(PIN_LIGHT_TREE_RELAY_4, !LIGHT_TREE_RELAY_ON);
  #endif
  #if (LIGHT_TREE_STRIP)
    for(int i=0;i<NUM_LEDS;i++) {
      leds[i] = CRGB::Black;
    }
    FastLED.show();
  #endif
}

void LightTree::abort() {
  #if (LIGHT_TREE_RELAY_ENABLE)
    digitalWrite(PIN_LIGHT_TREE_RELAY_1, !LIGHT_TREE_RELAY_ON);
    digitalWrite(PIN_LIGHT_TREE_RELAY_2, LIGHT_TREE_RELAY_ON);
    digitalWrite(PIN_LIGHT_TREE_RELAY_3, LIGHT_TREE_RELAY_ON);
    digitalWrite(PIN_LIGHT_TREE_RELAY_4, !LIGHT_TREE_RELAY_ON); 
  #endif
  #if (LIGHT_TREE_STRIP)
    for(int i=0;i<NUM_LEDS;i++) {
        leds[i] = CRGB::Red;
    }
    FastLED.show();
  #endif
}

void LightTree::ready() {
  led_reset();
  #if (LIGHT_TREE_STRIP)
    leds[0] = CRGB::Blue;
    FastLED.show();
  #endif
}

void LightTree::set_status(uint32_t color) {
  #if (LIGHT_TREE_STRIP)
    leds[0] = color;
    FastLED.show();
    //FastLED.setBrightness(BRIGHTNESS); // may be needed if builtin LED affecting
  #endif  
}

void LightTree::light_set(int step) {
  serial_print_val("Set LED ", step);
  #if (LIGHT_TREE_RELAY_ENABLE)
    switch (step) {
      case 1:
        digitalWrite(PIN_LIGHT_TREE_RELAY_1, LIGHT_TREE_RELAY_ON);
        break;
      case 2:
        digitalWrite(PIN_LIGHT_TREE_RELAY_2, LIGHT_TREE_RELAY_ON);
        break;
      case 3:
        digitalWrite(PIN_LIGHT_TREE_RELAY_3, LIGHT_TREE_RELAY_ON);
        break;
      case 4:
        digitalWrite(PIN_LIGHT_TREE_RELAY_4, LIGHT_TREE_RELAY_ON);
        break;
      default:
        led_reset();
    }
  #endif
  #if (LIGHT_TREE_STRIP)
  
      int position = 4-step;    // bottom of strip up
      // int position = step-1; // top of strip down
       
      switch (step) {       
        case 1:
          // set team Red LEDs
          for(int i=0;i<team_size;i++) {
            leds[i+position*team_size] = CRGB::Red;
          }
          break; 
        case 2:
          // set team Orange LEDs
          for(int i=0;i<team_size;i++) {
            leds[i+position*team_size] = CRGB::Orange;
          }
          break;
        case 3:
          // set 2nd team Orange LEDs 
          for(int i=0;i<team_size;i++) {
            leds[i+position*team_size] = CRGB::Orange;
          }
          break;
        case 4:
          //set team Green LEDs
          for(int i=0;i<team_size;i++) {
            leds[i+position*team_size] = CRGB::Green;
          }
          break;
        default:
          led_reset();
      }
  FastLED.show();
  #endif
}
