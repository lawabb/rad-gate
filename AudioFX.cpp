/*****************************************
 *  RemoteArDuino (RAD) Gate
 *  (c) Danny Frencham 2017
 *****************************************/

#include <SoftwareSerial.h>
#include "Adafruit_Soundboard.h"
#include <JQ6500_Serial.h>
#include "constants.h"
#include "utility.h"
#include "AudioFX.h"
#include "Gate.h"

#ifdef HARDWARE_SOUNDBOARD_ADAFRUIT
  bool SFX_ADAFRUIT = 1;
#endif
#ifdef HARDWARE_SOUNDBOARD_JQ6500
  bool SFX_ADAFRUIT = 0;
#endif

uint32_t offset;
uint32_t now;
uint32_t wait_time;


AudioFX::AudioFX(Adafruit_Soundboard *sbref, Gate* gateObj) {
  sfx = sbref;
  gate = gateObj;
}

AudioFX::AudioFX(JQ6500_Serial *jref, Gate* gateObj) {
  jfx = jref;
  gate = gateObj;
}

void AudioFX::exitable_timer(uint32_t offset, uint32_t wait_time) {
   
  if (wait_time == 0) {
    return;
  }
  while (wait_time > now) {
    if (gate->is_aborted()) {
      serial_print("Stop play");
      stop_play(); 
      return;
    }  
    now = millis() - offset;
  }
}

void AudioFX::start_tone(int hz) {
  // serial_print_val("Tone", TONE_DROP_HZ);
  tone(PIN_SPEAKER, TONE_DROP_HZ);
}

void AudioFX::stop_tone() {
  // serial_print("Tone Stop");
  noTone(PIN_SPEAKER);
}

void AudioFX::play_power_on() {
  // added extra tone for  major chord sound...
  tone(PIN_SPEAKER, 831, 100);
  delay(100);
  tone(PIN_SPEAKER, 1047, 100);
  delay(100);
  tone(PIN_SPEAKER, 1245, 600);
}

void AudioFX::play_sound_sample(uint8_t SFX_MESG, uint32_t wait_time) {
  if (!ENABLE_VOICE_CADENCE || gate->is_aborted()) {
    //serial_print("Skipping voice cadence");
    return;
  }
  if (wait_time != 0) {
    now = 0;
    offset = millis();
    serial_print_val("Play mp3 ", SFX_MESG);
  }
  play_sample(SFX_MESG);
  // Wait for (track + delay) = wait_time
  exitable_timer(offset, wait_time);
  //serial_print_val("SFX done Play", track);
}

void AudioFX::play_abort() {
  //serial_print("playing abort tones");
  tone(PIN_SPEAKER, TONE_ABORT_1_HZ, DELAY_ABORT_TONE_1_MS);
  delay(DELAY_ABORT_TONE_1_MS);
  tone(PIN_SPEAKER, TONE_ABORT_2_HZ, DELAY_ABORT_TONE_2_MS);
}

void AudioFX::play_sample(uint8_t track) {
  #if (SFX_ADAFRUIT)
    //serial_print("SFX playing on Adafruit device");
    sfx->playTrack(track);
    delay(50);
    while (digitalRead(PIN_SFX_ACT) == LOW) {
      // wait for sample to finish
    }
  #else
    // serial_print("SFX playing on JQ6500 device");
    jfx->playFileByIndexNumber(track);
  #endif
}

void AudioFX::stop_play() {
  #if (SFX_ADAFRUIT)
    //serial_print("Stop playing Adafruit device");
    sfx->stop();
  #else 
    //serial_print("Stop playing JQ6500 device");
    jfx->pause();
  #endif 
}
