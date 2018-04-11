/*****************************************
 *  RemoteArDuino (RAD) Gate
 *  (c) Danny Frencham 2017
 *****************************************/

#include "Gate.h"
#include "LightTree.h"
#include "AudioFX.h"
#include "Display.h"

#ifndef _SEQUENCE_H
#define _SEQUENCE_H

class Sequence {

  public:
    Sequence(Gate* gateOb, AudioFX* audioOb, LightTree* lighttreeOb, Display* displayOb);
    void begin_sequence();
    void abort_seq();
    void set_ready();
    void end_seq(volatile uint32_t react_time);
    
  private:
    Gate *gate;
    AudioFX *audio;
    LightTree *lighttree;
    Display *display;
};

#endif
