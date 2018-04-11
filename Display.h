
#include "constants.h"

#ifndef _DISPLAY_H
#define _DISPLAY_H


class Display {

  public:
    Display();
    void displayNumber(uint32_t value, bool leadingZero=1, uint8_t dp=0);
    void allOff();
    void setDisplay(uint8_t digits[],  uint8_t dp, int num_digits);  
    
  private:

};

#endif
