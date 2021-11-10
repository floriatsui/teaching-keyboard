#include "final_fsm.h"

// FSM variables
int curr_song_index;
int num_correct_keys, saved_clock, countdown;

// Global song constants
int  song_end;
int[] note_durations;
int[] song_notes;

state CURRENT_STATE;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  initialize_system();

  //calibrate();
  // Change to 7, 8, 9 based on calibration
  cap_sensors[G] = 8;
  cap_sensors[A] = 9;
  cap_sensors[B] = 7;
  // Change based on calibration
  thresholds[G] = 750;
  thresholds[A] = 750;
  thresholds[B] = 2400;

  //test_calibration();
  
  CURRENT_STATE = (state) 1;
  curr_song_index = 0;
  saved_clock = millis();
  countdown = 3;
  num_correct_keys = 0;

  // TODO: add song parsing here

  test_all_tests();
}

void loop() {

  update_inputs();
  set_mode();
  CURRENT_STATE = update_fsm(CURRENT_STATE, millis(), num_keys, last_key, curr_mode);
  delay(10);
  
}

state update_fsm(state cur_state, long mils, int num_keys, int last_key, mode curr_mode) {
  state next_state;
  switch(cur_state) {
    case sWAIT_FOR_MODE:
      break;
    case sDEMO = 2:
      break;
    case sLEARNING_COUNTDOWN:
      break;
    case sWAIT_FOR_KEY_LEARNING:
      break;
    case sKEY_PRESSED_LEARNING:
      break;
    case sTESTING_COUNTDOWN:
      break;
    case sWAIT_FOR_KEY_TESTING:
      break;
    case sKEY_PRESSED_TEACHING:
      break;
    case sNO_KEY_PRESSED_TEACHING:
      break;
    case sGAME_OVER:
      break;
  }
  return next_state;
}
