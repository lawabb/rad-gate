/*****************************************
 *  RemoteArDuino (RAD) Gate
 *  (c) Danny Frencham 2017
 *****************************************/

#include "constants.h"
#include "utility.h"
#include "Sequence.h"

int T0 = 0;
int T1 = T0 + DELAY_DROP_TONE_MS;
int T2 = T1 + DELAY_INTERVAL_MS;
int T3 = T2 + DELAY_DROP_TONE_MS;
int T4 = T3 + DELAY_INTERVAL_MS;
int T5 = T4 + DELAY_DROP_TONE_MS;
int T6 = T5 + DELAY_INTERVAL_MS;
int T7 = T6 + GATE_ACTIVE_MS;
int T8 = T6 + DELAY_DROP_TONE_FINAL_MS;

#if SHORT_PULSE_GATE
int target[] = {T0, T1, T2, T3, T4, T5, T6, T7, T8};
uint8_t count = 9;
int seq[][3] = { 
  { 1, 1,-1},
  {-1, 0,-1},
  { 2, 1,-1}, 
  {-1, 0,-1},
  { 3 ,1,-1},
  {-1, 0,-1},
  { 4, 1, 1},
  {-1,-1, 0},
  {-1, 0,-1}
  };

#else
int target[] = {T0, T1, T2, T3, T4, T5, T6, T8};
uint8_t count = 8;
int seq[][3] = { 
  { 1, 1,-1},
  {-1, 0,-1},
  { 2, 1,-1}, 
  {-1, 0,-1},
  { 3 ,1,-1},
  {-1, 0,-1},
  { 4, 1, 1},
  {-1, 0,-1}
  };
}
#endif

// for reaction time voice announce
// map individual digits (SFX_CHAR), to voice file numbers (SFX_MESG)
// SFX_CHAR, SFX_MESG

sfx_map sfx_maps[] =  {
  {'0', SFX_ZERO},
  {'1', SFX_ONE},
  {'2', SFX_TWO},
  {'3', SFX_THREE},
  {'4', SFX_FOUR},
  {'5', SFX_FIVE},
  {'6', SFX_SIX},
  {'7', SFX_SEVEN},
  {'8', SFX_EIGHT},
  {'9', SFX_NINE},  
  {'.', SFX_POINT}, 
  };

Sequence::Sequence(Gate* gateOb, AudioFX* audioOb, LightTree* lighttreeOb, Display* displayOb) {

  gate = gateOb;
  audio = audioOb;
  lighttree = lighttreeOb;
  display = displayOb;
}

void Sequence::begin_sequence() {
    
  uint8_t step = 0;
  uint32_t offset;
  uint32_t now = 0;
  int8_t val;
  volatile uint32_t react_time;

  serial_print("Sequence begin");
  display->allOff();
  gate->set_sequence_running(true);

  // digitalWrite(LED_BUILTIN, HIGH);  // turn on builtin LED. Note: also turns on Neo Pixel LED 0 green!
  // lighttree->set_status(0x000000);    // If using builtin LED can turn lightstrip led 0 off
  
  #if LIGHT_TREE_RELAY_ENABLE
  lighttree->led_reset(); 
  #endif
  #if LIGHT_TREE_STRIP
  lighttree->set_status(0xFFFF00); // or other color eg Yellow        
  #endif
  
  digitalWrite(PIN_LED_ACTIVE, HIGH);  // turn on "Active" LED

  audio->play_sound_sample(SFX_PREP, 3300);  // 1500 for sample + 1800 delay
  audio->play_sound_sample(SFX_WATCH_GATE, 2000);  // 2000 for sample and no delay
  
  gate->random_wait();

  // once the sequence starts, do not allow abort
  serial_print("No longer abortable");
  gate->set_abortable(false); 

  offset = millis();
  
  while((step < count) && (!gate->is_aborted())) {
    
    while (target[step] > now) {
      now = millis() - offset;
    }

    for (int i=0; i<3; i++){
      val = seq[step][i];
      // -1 means ignore
          
      if (val >= 0){
        // 0 = lightree
        if (i == 0){
          lighttree->light_set(val);
        }
        // 1 = audio
        if (i == 1){
          if (val == 0) {
            audio->stop_tone();
          }
          else if (val == 1) {
            audio->start_tone(TONE_DROP_HZ);
          }
        }
        // 2 = gate drop
        if (i == 2){
          if (val == 1) {
            GATE_DROP_START = millis();
            gate->drop(); // turn on solenoid
            serial_print("Waiting for reaction time...");
          }
          else if (val == 0) {
            gate->arm(); // turn off solenoid
          }
        }           
      }
    }
    step++;
  }
    
  serial_print("Setting sequence to STOP");
  gate->set_sequence_running(false);

  if (!gate->is_aborted()) {
    audio->stop_tone();
  }
  else {
    abort_seq();
    gate->set_abortable(true);
  }
  set_ready();

  digitalWrite(PIN_LED_ACTIVE, LOW);
  // digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(PIN_RELAY, !GATE_ACTIVE_LEVEL); // turn on magnet
  serial_print("Sequence complete");  
}

void Sequence::abort_seq() {
  serial_print("Sequence abort");
  lighttree->abort();
  audio->play_abort();
  //lighttree->led_reset(); // hold off reseting LEDs. Reset in set_ready()
  delay(3000);
  set_ready();
}

void Sequence::set_ready() {
  lighttree->ready();
  gate->ready();
}
         
void Sequence::end_seq(volatile uint32_t react_time) { 
  
  // convert reaction time from  milliseconds to array of chars
  // eg 1234 to '1' '.' '2' '3' '4' then play 'matching' files 'SFX_ONE' etc
  
  serial_print_val("Reaction Time =", react_time);
  
  double b = react_time/1000.0;
  String s = String(b, 3);
  int n = s.length();

  // Write react time to seven segment display
  display-> displayNumber(react_time, 1, n-4); // n-4 gives which digit dp is attached to
  
  char char_array[n+1]; 
  strcpy(char_array, s.c_str());

  for (int i=0; i<n+1; i++) {   
    for (int j=0; j<11; j++) {
      if (sfx_maps[j].SFX_CHAR == char_array[i]) {
        audio->play_sound_sample(sfx_maps[j].SFX_MESG);
        break;
      }     
    }
  }
  audio->play_sound_sample(SFX_SECONDS);
  serial_print("Press GO to restart...");
}   

