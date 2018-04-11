/*****************************************
 *  RemoteArDuino (RAD) Gate
 *  
 *****************************************/

#include "Display.h"
#include <stdint.h>
#include <Arduino.h>
#include "utility.h"

uint8_t num_digits = 4;
uint8_t msb, nmsb, nlsb,lsb;
uint8_t digit[] = {msb, nmsb, nlsb,lsb};
uint8_t dp;
uint8_t blank = 11;  // 0x00

int LED_SEG_TAB[] = {
  0xfc, // 0
  0x60, // 1
  0xda, // 2
  0xf2, // 3
  0x66, // 4
  0xb6, // 5
  0xbe, // 6 
  0xe0, // 7
  0xfe, // 8
  0xf6, // 9
  0x01, // .    // 10
  0x00, // Off  // 11
  0x02, // -    // 12
  0x0a, // r    // 13
  0xee, // A    // 14
  0x7a  // d    // 15
 };
 

Display::Display() {}

void Display::displayNumber(uint32_t value, bool leadingZero, uint8_t dp) {
  
  // leadingZero = 1 means show leading zeros
  // dp = 0 means no dp (so +0 to LED_SEG_TAB[x])
  // dp = 1-> 4  means display dp only for that corresponding digit (so +1 to LED_SEG_TAB[x])
  
  // chop off excess digits as required
  // could round first, but is not neccessary
  if (value > 999999){
    value = value/1000; 
  } else 
  if (value > 99999){
    value = value/100; 
  } else 
  if (value > 9999) {  
    value = value/10; 
  }
 
  // break down integer "value" into single digits as msb, nmsb, nlsb, lsb 
  msb = value / 1000 ;
  value = value % 1000;
  digit[0] = msb;
  
  nmsb = value / 100;
  value = value % 100;
  digit[1] = nmsb;
  
  nlsb = value / 10;
  value = value % 10;
  digit[2] = nlsb;
  
  lsb = value;
  digit[3] = lsb;

  // progressively remove leading zeros (if required)
  if (!leadingZero) { 
 
    if (msb==0) {
      digit[0] = blank;
    } 
    if (msb==0 && nmsb==0 ) {
      digit[1] = blank;
    }
    if (msb==0 && nmsb==0 && nlsb==0) {
      digit[2] = blank;
    }
    if (msb==0 && nmsb==0 && nlsb==0 && lsb==0) {
      digit[3] = blank;
    }
  }
  setDisplay(digit, dp, num_digits);
}

void Display::allOff() { // turns off all segments 
  digitalWrite(PIN_DISPLAY_LATCH, LOW);
  for (int y=0; y<num_digits; y++) {
    shiftOut(PIN_DISPLAY_DATA, PIN_DISPLAY_CLOCK, LSBFIRST, 0);  
  }
  digitalWrite(PIN_DISPLAY_LATCH, HIGH);
}

void Display::setDisplay(uint8_t digits[], uint8_t dp, int num_digits) {
  // Set the Display from array
  digitalWrite(PIN_DISPLAY_LATCH, LOW);
  for (int y=0; y<num_digits; y++) {   
    shiftOut(PIN_DISPLAY_DATA, PIN_DISPLAY_CLOCK, MSBFIRST, LED_SEG_TAB[digits[y]] + (!(dp==0 || (dp-1) != y)));
  }    
  digitalWrite(PIN_DISPLAY_LATCH, HIGH);
}

