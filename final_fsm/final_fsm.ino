#include "final_fsm.h"
#include "pitches.h"

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
    case sWAIT_FOR_MODE: // 1
      if (mils - saved_clock >= 5000 && curr_mode == LEARNING) { // 1-2
        display("LEARNING DEMO"); 
        saved_clock = mils; 
        curr_song_index = 0; 
        next_state = sDEMO; 
      } else if (mils - saved_clock >= 5000 && curr_mode == TESTING) { // 1-6
        display("BEGIN TESTING MODE"); 
        saved_clock = mils; 
        curr_song_index = 0; 
        next_state = sTESTING_COUNTDOWN; 
      } else {
        next_state = sWAIT_FOR_MODE; 
      }
      break;
    case sDEMO: // 2
      if (curr_song_index < song_end) { // 2-2
        play_demo_note(song_notes[curr_song_index], note_durations[curr_song_index]); 
        saved_clock = mils; 
        curr_song_index += 1; 
        next_state = sDEMO; 
      } else if (curr_song_index == song_end) { // 2-3
        display("BEGIN LEARNING MODE"); 
        curr_song_index = 0; 
        countdown = 3; 
        next_state = sLEARNING_COUNTDOWN; 
      }
      break;
    case sLEARNING_COUNTDOWN: // 3
      if (countdown > 0 && mils - saved_clock >= 1000) { // 3-3
        display(countdown);
        countdown -= 1;
        saved_clock = mils;
        next_state = sTESTING_COUNTDOWN;
      } else if (countdown <= 0 && mils - saved_clock >= 1000) { // 3-4
        display("BEGIN PLAYING"); 
        dim_led(song_notes[curr_song_index], note_durations[curr_song_index], GREEN); 
        countdown = 3; 
        next_state = sWAIT_KEY_LEARNING; 
      }
      break;
    case sWAIT_FOR_KEY_LEARNING: // 4
      if (num_keys == 0 && curr_song_index < song_end && 
      mils - saved_clock >= note_durations[curr_song_index]) { // 4-4
        dim_led(song_notes[curr_song_index+1], note_durations[curr_song_index+1], GREEN); 
        display(String(curr_song_index + 2) + "/" + String(song_end)); 
        curr_song_index += 1; 
        saved_clock = mils; 
        next_state = sWAIT_FOR_KEY_LEARNING; 
      } else if (num_keys == 1 && curr_song_index < song_end) { // 4-5
        play_note(last_key, note_durations[curr_song_index], saved_clock); 
        light_led(song_notes[curr_song_index], note_durations[curr_song_index], GREEN); 
        saved_clock = mils; 
        next_state = sKEY_PRESSED_LEARNING; 
      } else if (curr_song_index == song_end) { // 4-1 
        display("LEARNING MODE OVER"); 
        countdown = 3; 
        saved_clock = mils; 
        next_state = sWAIT_FOR_MODE; 
      }
      break;
    case sKEY_PRESSED_LEARNING: // 5
      if (mils - saved_clock >= note_durations[curr_song_index]) { // 5-4
        dim_led(song_notes[curr_song_index+1], note_durations[curr_song_index+1], GREEN); 
        display(String(curr_song_index + 1) + "/" + String(song_end)); 
        saved_clock = mils; 
        curr_song_index += 1; 
        next_state = sWAIT_FOR_KEY_LEARNING; 
      }
      break;
    case sTESTING_COUNTDOWN: // 6
      if (countdown > 0 && mils - saved_clock >= 1000) {
        display(countdown);
        countdown -= 1;
        saved_clock = mils;
        next_state = sTESTING_COUNTDOWN;
      } else if (countdown <= 0 && mils - saved_clock >= 1000) {
        display(countdown);
        countdown = 3;
        next_state = sWAIT_FOR_KEY_TESTING;
      } else {
        next_state = sTESTING_COUNTDOWN;
      }
      break;
    case sWAIT_FOR_KEY_TESTING: // 7 
      if(num_keys = 1 && curr_song_index < song_end && last_key = notes[curr_song_index]){ // 7-8(a)
        // PLAY NOTE-- DURATION??
        play_note(last_key, note_durations[curr_song_index], saved_clock);
        light_led(song_notes[curr_song_index], GREEN);
        saved_clock = mils;
        num_correct_notes++;
        next_state = sKEY_PRESSED_TESTING;
      } else if (num_keys = 1 && curr_song_index < song_end && last_key != notes[curr_song_index]) { // 7-8(b)
        play_note(last_key, note_durations[curr_song_index], saved_clock);
        light_led(song_notes[curr_song_index], RED);
        saved_clock = mils;
        next_state = sKEY_PRESSED_TESTING;
      } else if (num_keys = 0 && curr_song_index < song_end && mils - saved_clock >=note_durations[curr_song_index] / 2) { // 7-9
        light_led(song_notes[curr_song_index], RED);
        next_state = sNO_KEY_PRESSED_TESTING;
      } else if (curr_song_index = song_end) { // 7-10
        display(num_correct_notes, song_end);
        next_state = sGAME_OVER;
      } else {
        next_state = sWAIT_FOR_KEY_TESTING;
      }
      break;
    case sKEY_PRESSED_TESTING: // 8 
      if (mils - saved_clock >= note_durations[curr_song_index]) {
        display(curr_song_index + 1 + “/” + song_end + 1);
        reset_keys();
        curr_song_index += 1;
        saved_clock = mils;
        next_state = sWAIT_FOR_KEY_TESTING;
      } else {
        next_state = sKEY_PRESSED_TESTING:
      }
        break;
    case sNO_KEY_PRESSED_TEACHING: // 9 
      if (curr_song_index < song_end and mils - saved_clock >=note_durations[curr_song_index]) {
        display(curr_song_index + “/” + song_end);
        curr_song_index += 1;
        saved_clock = mils;
        next_state = sWAIT_FOR_KEY_TESTING;
      } else {
        next_state = sNO_KEY_PRESSED_TESTING;
      }
      break;
    case sGAME_OVER: // 10
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
