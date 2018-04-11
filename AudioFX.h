/*****************************************
 *  RemoteArDuino (RAD) Gate
 *  (c) Danny Frencham 2017
 *****************************************/

#include "Adafruit_Soundboard.h"
#include <JQ6500_Serial.h>
#include "Gate.h"

#ifndef _AUDIO_H
#define _AUDIO_H

class AudioFX {

  public:
    AudioFX(Adafruit_Soundboard *sbref);
    AudioFX(JQ6500_Serial *jref, Gate *gateObj);
    void start_tone(int hz);
    void stop_tone();
    void play_sound_sample(uint8_t SFX_MESG, uint32_t wait_time=0);
    void play_abort();
    void play_power_on();
    void stop_play();

  private:
    Adafruit_Soundboard *sfx;
    JQ6500_Serial *jfx;
    Gate *gate;
    void play_sample(uint8_t track);
    void exitable_timer(uint32_t offset, uint32_t wait_time);
};

#endif
