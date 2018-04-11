/*****************************************
 *  RemoteArDuino (RAD) Gate
 *  (c) Danny Frencham 2017
 *****************************************/

#include "constants.h"
#include "utility.h"
#include "Gate.h"

bool FLAG_ABORT_PENDING = false;
bool FLAG_SEQUENCE_RUNNING = false;
bool FLAG_IS_ABORTABLE = false;

// FLAG_GATE_DOWN see constants.cpp

Gate::Gate(){}

void Gate::random_wait() {
  int rand_delay = random(DELAY_RAND_MIN, DELAY_RAND_MAX+1);
  uint32_t wait_timer = millis() + rand_delay;
  // serial_print_val("Random wait = ",rand_delay);
  // serial_print_val("Wait timer", wait_timer);
  // serial_print_val("Millis",millis());
  while((millis() < wait_timer) && (!FLAG_ABORT_PENDING)) {
    // waiting for random wait to finish
  }
  //serial_print("Wait timer done");
}

void Gate::ready(){
  FLAG_ABORT_PENDING = false;
  FLAG_IS_ABORTABLE = false;
}

void Gate::arm() {
  //serial_print("Deactivate solenoid");
  digitalWrite(PIN_RELAY, !GATE_ACTIVE_LEVEL);
}

void Gate::abort() {
  FLAG_ABORT_PENDING = true;
}

void Gate::drop() {
  FLAG_GATE_DOWN  = true;
  serial_print("Gate Drop");     // Activate solenoid or Deactivate electromagnet
  digitalWrite(PIN_RELAY, GATE_ACTIVE_LEVEL);
}

bool Gate::is_sequence_running() {
  return FLAG_SEQUENCE_RUNNING;
}

bool Gate::is_aborted() {
  return FLAG_ABORT_PENDING;
}

bool Gate::is_abortable() {
  return FLAG_IS_ABORTABLE && !FLAG_ABORT_PENDING;
}

void Gate::set_sequence_running(bool running) {
  FLAG_SEQUENCE_RUNNING = running;
  FLAG_IS_ABORTABLE = true;
}

void Gate::set_abortable(bool abortable) {
  FLAG_IS_ABORTABLE = abortable;
}


