/*****************************************
 *  RemoteArDuino (RAD) Gate
 *  (c) Danny Frencham 2017
 *****************************************/

#include "utility.h"

#ifndef _LIGHTREE_H
#define _LIGHTREE_H

class LightTree {

  public:

    LightTree();
    void initialise();
    void light_set(int step);
    void led_reset();
    void abort();
    void ready();
    void set_status(uint32_t color);
    
  private:
    
};

#endif
