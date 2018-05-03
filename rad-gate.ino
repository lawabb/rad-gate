/*****************************************
 *  RemoteArDuino (RAD) Gate
 *  (c) Danny Frencham 2017
 *****************************************/

 /********************************************
 *  Notes: 
 *  Edit constants.h and constants.cpp to set 
 *  various configuration options
 *  
 *********************************************/

#include <SoftwareSerial.h>
#include "constants.h"
#include "utility.h"
#include "LightTree.h"
#include "Gate.h"
#include "Sequence.h"
#include "Display.h"
#include <JQ6500_Serial.h>

// declarations
bool buttonPressed = false;
bool got_interrupt1 = false;
bool do_end_seq = false;
uint32_t interrupt1_time = 0;
uint32_t lastButtonPress = 0;
int vdig = 0;


Gate gate = Gate();

#ifdef HARDWARE_SOUNDBOARD_ADAFRUIT
  SoftwareSerial ss = SoftwareSerial(PIN_SFX_TX, PIN_SFX_RX);
  Adafruit_Soundboard sfx = Adafruit_Soundboard(&ss, NULL, PIN_SFX_RST);
  AudioFX audioFX = AudioFX(&sfx);
#endif
#ifdef HARDWARE_SOUNDBOARD_JQ6500
  JQ6500_Serial mp3(PIN_SFX_TX, PIN_SFX_RX);
  AudioFX audioFX = AudioFX(&mp3, &gate);
#endif

LightTree lighttree = LightTree();
Display display = Display();
Sequence sequence = Sequence(&gate, &audioFX, &lighttree, &display);


void setup() 
{
  Serial.begin(SERIAL_RATE);
  serial_print(VERSION);

  #ifdef HARDWARE_SOUNDBOARD_ADAFRUIT
    ss.begin(9600);
    pinMode(PIN_SFX_ACT, INPUT);
    serial_print("Adafruit Sound board initialised");
  #endif
  #ifdef HARDWARE_SOUNDBOARD_JQ6500
    mp3.begin(9600);
    mp3.reset();
    mp3.setVolume(SFX_VOLUME);
    mp3.setLoopMode(MP3_LOOP_NONE);
    serial_print("JQ6500 initialised");
  #endif

  // pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIN_LED_ACTIVE, OUTPUT);
  pinMode(PIN_SPEAKER, OUTPUT);
  pinMode(PIN_BUTTON_GO, INPUT);
  pinMode(PIN_RELAY, OUTPUT);
  pinMode(PIN_ANA_VIN, INPUT);
  pinMode(PIN_BAT_MON, INPUT);
  pinMode(PIN_SENSOR, INPUT_PULLUP);

  // Add more randomness otherwise uses same random sequence on each startup
  randomSeed(analogRead(PIN_ANA_VIN));

  // Use interrupt0 to catch ABORT request from GO button
  attachInterrupt(digitalPinToInterrupt(PIN_BUTTON_GO), Interrupt0, HIGH);
  
  // Use Interrupt1 from light beam trigger input for reaction time measurement
  // attach_interrupt1(); // Attaching in main loop()

  // ensure solenoid or magnet is in default condition
  digitalWrite(PIN_RELAY, !GATE_ACTIVE_LEVEL);
  
  lighttree.initialise();
  lighttree.ready();

  // 4 digit LED display
  //set pins to output
  pinMode(PIN_DISPLAY_LATCH, OUTPUT);
  pinMode(PIN_DISPLAY_CLOCK, OUTPUT);
  pinMode(PIN_DISPLAY_DATA, OUTPUT); 

  // Display start message " rAd"
  uint8_t nameArray[] = {11, 13, 14, 15};
  display.setDisplay(nameArray, 0, 4);

  // Play 'ready to roll' tones
  audioFX.play_power_on();
  
  //serial_print("Initialised");
  serial_print("Waiting for GO Button..");
}

void Interrupt0()
{
  if (gate.is_sequence_running()) {
    if ((millis() - lastButtonPress) > 1000) {
      if (gate.is_abortable()&& !gate.is_aborted()) { 
        gate.abort();  // also causes audio play to stop
        lighttree.abort();                               
      }
    }   
  }
} 

void Interrupt1()
{
  if (FLAG_GATE_DOWN && !got_interrupt1) {
    // detach interupt to prevent retrigger - attach again in main loop() on rerun
    // run end_seq() from main loop as is problematic from here
    interrupt1_time = millis();
    detachInterrupt(1);
    got_interrupt1 = true;
    do_end_seq = true;
  }  
}

void attach_interrupt1() {
  attachInterrupt(digitalPinToInterrupt(PIN_SENSOR), Interrupt1, RISING);
}

void loop()
{
  if (digitalRead(PIN_BUTTON_GO) == LOW) {
    //serial_print("LOW");
    buttonPressed = 0; // reset
  }
  else if (digitalRead(PIN_BUTTON_GO) == HIGH) {
    serial_print("GO!");
    if (!buttonPressed) {
      buttonPressed = 1;
      lastButtonPress = millis();
      // serial_print_val("Sequence start - lastButtonPress val", lastButtonPress);
      FLAG_GATE_DOWN = false;
      got_interrupt1 = false;
      attach_interrupt1();
      sequence.begin_sequence();
    }
  }

  // optionally get 'reaction time' value
  if (do_end_seq) {
    do_end_seq = false;
    sequence.end_seq(interrupt1_time - GATE_DROP_START);
  }

  // Battery monitor
  // Checks the voltage and if too low gives alarm
  // Set jumper on board to 5V to stop alarms while testing
  // Using 100K:10K voltage divider on IP giving  Vin = Vdc/11
  // Trim the 100K value for accuracy - I used 82K + 50K trimpot
  // Battery Alarm is set at Voltage (mV) BAT_ALARM_VOLTS
 
  vdig = analogRead(PIN_BAT_MON);
  // serial_print_val("Battery volatage is (mV)", vdig*55.0/1.023); 
  // delay(1000);  // add some delay when testing
  
  while (vdig < 1.023*BAT_ALARM_VOLTS/55) {    
      serial_print_val("Battery volatage is (mV)", vdig*55.0/1.023);
      
      if (vdig*55.0/1.023 < 10000) {
        display.displayNumber(vdig*55.0/1.023, 1, 1); //  DP for numbers < 10.00
      } else {
        display.displayNumber(vdig*55.0/1.023, 0, 2); //  DP for numbers > 10.00      
      }
  
      audioFX.stop_play();
      delay(500);
      audioFX.play_sound_sample(SFX_LOW_BAT_ALM);
      delay(5000);
      display.allOff();
      vdig = analogRead(PIN_BAT_MON);
      
  }
}
