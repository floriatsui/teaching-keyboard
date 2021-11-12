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
    //6
    case sTESTING_COUNTDOWN:
      if (countdown >= 0 and mils - saved_clock >= 500) {
        display(countdown);
        countdown -= 1;
        saved_clock = mils;
        next_state = sTESTING_COUNTDOWN;
      } else if (countdown < 0 and mils - saved_clock >= 500) {
        display(countdown);
        countdown = 3;
        next_state = sWAIT_FOR_KEY_TESTING;
      } else {
        next_state = sTESTING_COUNTDOWN;
      }

      break;

    //7 
    case sWAIT_FOR_KEY_TESTING:
     if (num_keys = 0 and curr_song_index < song_end and mils - saved_clock >=note_durations[curr_song_index]){
        display(curr_song_index + 1 + “/” + song_end + 1);
        curr_song_index += 1;
        saved_clock := mils;
        next_state = sWAIT_FOR_KEY_TESTING;
      } else if(num_keys = 1 and curr_song_index < song_end and last_key = notes[curr_song_index]){
        // PLAY NOTE-- DURATION??
        play_note(last_key, );
        light_led(song_notes[curr_song_index], GREEN);
        curr_song_index += 1;
        saved_clock = mils;
        num_correct_notes++;
        next_state = sKEY_PRESSED_TEACHING;
      } else if (num_keys = 1 and curr_song_index < song_end and last_key != notes[curr_song_index]) {
        // ?? check play_note specs
        play_note(last_key, );
        light_led(song_notes[curr_song_index], RED);
        curr_song_index += 1;
        saved_clock = mils;
        next_state = sKEY_PRESSED_TEACHING;
      } else if (num_keys = 0 and curr_song_index < song_end and mils - saved_clock >=note_durations[curr_song_index] / 2) {
        //play_note(song_notes[curr_song_index], note_durations[curr_song_index]);
        light_led(song_notes[curr_song_index], RED);
        next_state = sNO_KEY_PRESSED_TEACHING;
      } else if (curr_song_index = song_end) {
        display(num_correct_notes, song_end + 1);
        next_state = sGAME_OVER;
      } else {
        next_state = sWAIT_FOR_KEY_TESTING;
      }
      break;
      
    //8 
    case sKEY_PRESSED_TEACHING:
      if (mils - saved_clock >= note_durations[curr_song_index]) {
        display(curr_song_index + 1 + “/” + song_end + 1);
        reset_keys();
        curr_song_index += 1;
        saved_clock = mils;
        next_state = sWAIT_FOR_KEY_TESTING;
    } else {
      next_state = sKEY_PRESSED_TEACHING:
    }
      break;
      
    // 9 
    case sNO_KEY_PRESSED_TEACHING:
      if (curr_song_index < song_end and mils - saved_clock >=note_durations[curr_song_index]) {
        display(curr_song_index + “/” + song_end);
        curr_song_index += 1;
        saved_clock = mils;
        next_state = sWAIT_FOR_KEY_TESTING;
      } else {
        next_state = sNO_KEY_PRESSED_TEACHING;
      }
      break;

    // 10
    case sGAME_OVER:
      if (mils - saved_clock >= 10000) {
        display(TESTING MODE OVER);
        countdown = 3;
        saved_clock = mils;
        num_currect_notes = 0;
        next_state = sWAIT_FOR_MODE;
      } else {
        next_state = sGAME_OVER;
      }
      
      break;
  }
  return next_state;
}
