#include "final_fsm.h"

// FSM variables
int curr_song_index;
int num_correct_keys, saved_clock, countdown;

// Global song constants
int song_end = 17;
int note_durations[] = {2500, 2500, 5000, 2500, 2500, 5000, 1250, 1250, 1250, 1250, 1250, 1250, 1250, 1250, 2500, 2500, 5000};
int note_durations2[] = {800, 800, 1600, 800, 800, 1600, 400, 400, 400, 400, 400, 400, 400, 400, 800, 800, 1600};


int song_notes[] = {NOTE_B5, NOTE_A5, NOTE_G5, NOTE_B5, NOTE_A5, NOTE_G5, 
                    NOTE_G5, NOTE_G5, NOTE_G5, NOTE_G5, NOTE_A5,NOTE_A5,
                    NOTE_A5,NOTE_A5,NOTE_B5, NOTE_A5, NOTE_G5}; 

state CURRENT_STATE;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial1.begin(9600);

  initialize_system();

  //calibrate();
  // Change to 7, 8, 9 based on calibration
  cap_sensors[0] = 8;
  cap_sensors[1] = 9;
  cap_sensors[2] = 10;
  
  // Change based on calibration
  thresholds[0] = 650;
  thresholds[1] = 700;
  thresholds[2] = 600;

  keys[0] = NOTE_G5;
  keys[1] = NOTE_A5;
  keys[2] = NOTE_B5;

  //test_calibration();
  
  CURRENT_STATE = (state) 1;
  curr_song_index = 0;
  saved_clock = millis();
  countdown = 3;
  num_correct_keys = 0;

  PORT->Group[PORTB].DIRSET.reg |= (1<<PB_PIN);

   pinMode(Ggreen, OUTPUT);
   pinMode(Gred, OUTPUT);
   pinMode(Agreen, OUTPUT);
   pinMode(Ared, OUTPUT);
   pinMode(Bgreen, OUTPUT);
   pinMode(Bred, OUTPUT);
   
   toggleSwitch.setDebounceTime(200);


  NVIC_DisableIRQ(WDT_IRQn);
  NVIC_ClearPendingIRQ(WDT_IRQn);
  NVIC_SetPriority(WDT_IRQn, 0);
  NVIC_EnableIRQ(WDT_IRQn);

  // Configure and enable WDT GCLK:
  GCLK->GENDIV.reg = GCLK_GENDIV_DIV(6) | GCLK_GENDIV_ID(5);
  while (GCLK->STATUS.bit.SYNCBUSY);

  GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(5) | GCLK_GENCTRL_GENEN | GCLK_GENCTRL_SRC(3) | GCLK_GENCTRL_DIVSEL;
  while (GCLK->STATUS.bit.SYNCBUSY);
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(3) | GCLK_CLKCTRL_GEN(5) | GCLK_CLKCTRL_CLKEN;
  while (GCLK->STATUS.bit.SYNCBUSY);
  

  // Configure and enable WDT:
  WDT->CONFIG.reg = WDT_CONFIG_PER(9);
  WDT->EWCTRL.reg = WDT_EWCTRL_EWOFFSET(8);
  WDT->CTRL.reg = WDT_CTRL_ENABLE;
  WDT->INTENSET.reg = WDT_INTENSET_EW;

  //attachInterrupt(switchPin, update_mode, CHANGE);

  if (test_all_tests()){
    Serial.println("All tests passed");
  } else {
    Serial.println("At least 1 test failed");
  }
}

void loop() {

  curr_mode = LEARNING;
  update_inputs();
  CURRENT_STATE = update_fsm(CURRENT_STATE, millis(), num_keys, last_key, curr_mode);
  delay(100);
  
}

state update_fsm(state cur_state, long mils, int num_keys, int last_key, mode curr_mode) {
  state next_state;
  String curr_mode_for_print = "learning"; 
  switch(cur_state) {
    case sWAIT_FOR_MODE: // 1  
      set_mode();
    //Serial.println(curr_mode);
      if (curr_mode == TESTING) {
        curr_mode_for_print = "testing";
      }
      display_message("Mode: " + curr_mode_for_print);
      analogWrite(Ggreen, 0);
      reset_keys(); 
      //delay(500);  
      if (mils - saved_clock >= 10000 && curr_mode == LEARNING) { // 1-2
        WDT->CLEAR.reg = 0xA5;
        //display("LEARNING DEMO"); 
        Serial.println("LEARNING DEMO"); 
        display_message("STARTING DEMO");
        saved_clock = mils; 
        curr_song_index = 0; 
        next_state = sDEMO; 
      } else if (mils - saved_clock >= 10000 && curr_mode == TESTING) { // 1-6
      
        WDT->CLEAR.reg = 0xA5;
        display_message("START TESTING"); 
        Serial.println("BEGIN TESTING MODE"); 
        saved_clock = mils; 
        curr_song_index = 0; 
        next_state = sTESTING_COUNTDOWN; 
      } else {
        next_state = sWAIT_FOR_MODE; 
      }
      break;
    case sDEMO: // 2
      if (curr_song_index < song_end) { // 2-2
        WDT->CLEAR.reg = 0xA5;
        Serial.println("note duration: " + String(note_durations[curr_song_index]));
        if (curr_song_index != 0) {
          light_led(song_notes[curr_song_index-1], GREEN, 0); 
        }
        light_led(song_notes[curr_song_index], GREEN, 255); 
        play_demo_note(song_notes[curr_song_index], note_durations2[curr_song_index]); 
        saved_clock = mils; 
        curr_song_index += 1; 
        next_state = sDEMO; 
      } else if (curr_song_index == song_end) { // 2-3
        WDT->CLEAR.reg = 0xA5;
        display_message("START LEARNING"); 
        Serial.println("BEGIN LEARNING MODE"); 
        light_led(song_notes[song_end-1], GREEN, 0); 
        saved_clock = mils; 
        curr_song_index = 0; 
        countdown = 3; 
        next_state = sLEARNING_COUNTDOWN; 
      }
      break;
    case sLEARNING_COUNTDOWN: // 3
      Serial.println("in countdown"); 
      //delay(1000); 
      if (countdown > 0 && mils - saved_clock >= 1000) { // 3-3
        WDT->CLEAR.reg = 0xA5;
        display_message("Countdown " + String(countdown));
        Serial.println("printing countdown"); 
        Serial.println(countdown);
        countdown -= 1;
        saved_clock = mils;
        next_state = sLEARNING_COUNTDOWN; 
        //delay(500); 
      } else if (countdown <= 0 && mils - saved_clock >= 1000) { // 3-4
        WDT->CLEAR.reg = 0xA5;
        Serial.println("BEGIN PLAYING"); 
        light_led(song_notes[curr_song_index], GREEN, 255); 
        display_curr_index(1, song_end);
        //countdown = 3; 
        saved_clock = mils;
        reset_keys(); 
        next_state = sWAIT_FOR_KEY_LEARNING; 
      } else {
        next_state = sLEARNING_COUNTDOWN; 
      }
      break;
    case sWAIT_FOR_KEY_LEARNING: // 4
      Serial.println("in learning mode pls pause"); 
      //delay(500); 
      if (num_keys == 0 && curr_song_index <= (song_end - 1) && 
      mils - saved_clock >= note_durations[curr_song_index]) { // 4-4
        WDT->CLEAR.reg = 0xA5;
        Serial.println("taking transition 4 back to 4");
        // make sure led is off for current/previous note
        light_led(song_notes[curr_song_index], GREEN, 0); 
        
        if (curr_song_index != song_end - 1) {
          // light led for next note
          light_led(song_notes[curr_song_index+1], GREEN, 255); 
          display_curr_index(curr_song_index+2, song_end);
          Serial.println(String(curr_song_index + 2) + "/ " + String(song_end));
        }
        
        curr_song_index += 1; 
        saved_clock = mils; 
        next_state = sWAIT_FOR_KEY_LEARNING; 
      } else if (num_keys == 1 && curr_song_index <= (song_end - 1) && (mils - saved_clock) < note_durations[curr_song_index]) { // 4-5
        WDT->CLEAR.reg = 0xA5;
        Serial.println("taking transition 4 back to 5");
        Serial.println("note has been pressed"); 
        if (curr_song_index != 0) {
          light_led(song_notes[curr_song_index - 1], GREEN, 0); 
        }
        Serial.print("last key ");
        Serial.println(last_key); 
        light_led(song_notes[curr_song_index], GREEN, 100); 
        play_note(last_key, note_durations[curr_song_index], saved_clock, mils); 
        
        next_state = sKEY_PRESSED_LEARNING; 
      } else if (curr_song_index == song_end && (mils - saved_clock) >= note_durations[curr_song_index]) { // 4-1 
        WDT->CLEAR.reg = 0xA5;
        Serial.println("taking transition 4 back to 1");
        Serial.println("LEARNING MODE OVER");
        display_message("LEARNING DONE");
        analogWrite(Ggreen, 0);
        light_led(song_notes[curr_song_index - 1], GREEN, 0); 
        countdown = 3; 
        saved_clock = mils; 
        reset_keys();
        next_state = sWAIT_FOR_MODE; 
      } else {
        next_state = sWAIT_FOR_KEY_LEARNING; 
      }
      break;
    case sKEY_PRESSED_LEARNING: // 5
      if (mils - saved_clock >= note_durations[curr_song_index]) { // 5-4
        WDT->CLEAR.reg = 0xA5;
        Serial.println("taking transition 5 back to 4");
        light_led(song_notes[curr_song_index], GREEN, 0); 
        if (curr_song_index != (song_end - 1)) {
          light_led(song_notes[curr_song_index+1], GREEN, 255); 
          display_curr_index(curr_song_index+2, song_end);
        }
        saved_clock = mils; 
        curr_song_index += 1; 
        next_state = sWAIT_FOR_KEY_LEARNING; 
        reset_keys();
      } else {
        next_state = sKEY_PRESSED_LEARNING; 
      }
      break;
    case sTESTING_COUNTDOWN: // 6
      if (countdown > 0 && mils - saved_clock >= 1000) { // 6-6
        WDT->CLEAR.reg = 0xA5;
        display_message("Countdown " + String(countdown));
        Serial.println("TESTING countdown");
        Serial.println(countdown);
        countdown -= 1;
        saved_clock = mils;
        next_state = sTESTING_COUNTDOWN;
      } else if (countdown <= 0 && mils - saved_clock >= 1000) { // 6-7
        WDT->CLEAR.reg = 0xA5;
        Serial.println(countdown);
        display_curr_index(1, song_end);
        reset_keys(); 
        countdown = 3;
        next_state = sWAIT_FOR_KEY_TESTING;
      } else {
        next_state = sTESTING_COUNTDOWN;
      }
      break;
    case sWAIT_FOR_KEY_TESTING: // 7 
      Serial.println("WAIT FOR KEY TESTING"); 
      Serial.print("curr song index: "); 
      Serial.println(curr_song_index);
      
      if(num_keys == 1 && curr_song_index <= (song_end -1) && last_key == song_notes[curr_song_index]
          &&  (mils - saved_clock) < note_durations[curr_song_index] / 2){ // 7-8(a)-- correct
        WDT->CLEAR.reg = 0xA5;
        light_led(song_notes[curr_song_index], GREEN, 255);
        play_note(last_key, note_durations[curr_song_index], saved_clock, mils);
        num_correct_keys++;
        next_state = sKEY_PRESSED_TESTING;
      } else if (num_keys == 1 && curr_song_index <= (song_end -1) && last_key != song_notes[curr_song_index]
                  && (mils - saved_clock) < note_durations[curr_song_index] / 2) { // 7-8(b)
        WDT->CLEAR.reg = 0xA5;
        light_led(song_notes[curr_song_index], RED, 255);
        play_note(last_key, note_durations[curr_song_index], saved_clock, mils);
        next_state = sKEY_PRESSED_TESTING;
      } else if (num_keys == 0 && curr_song_index <= (song_end -1) && mils - saved_clock >= note_durations[curr_song_index] / 2) { // 7-9
        WDT->CLEAR.reg = 0xA5;
        light_led(song_notes[curr_song_index], RED, 255);
        next_state = sNO_KEY_PRESSED_TESTING;
      } else if (curr_song_index == song_end) { // 7-10
        WDT->CLEAR.reg = 0xA5;
        display_message("Score: " + String(num_correct_keys) + "/" + String(song_end));
        next_state = sGAME_OVER;
      } else {
        next_state = sWAIT_FOR_KEY_TESTING;
      }
      break;
    case sKEY_PRESSED_TESTING: // 8 
      if (mils - saved_clock >= note_durations[curr_song_index]) { // 8-7
        WDT->CLEAR.reg = 0xA5;
        Serial.println("going back to 7 from 8"); 
        light_led(song_notes[curr_song_index], RED, 0);
        light_led(song_notes[curr_song_index], GREEN, 0);
        if (curr_song_index != (song_end - 1)) {
          display_curr_index(curr_song_index+2, song_end);
        }
        Serial.println(String(curr_song_index + 1) + String(song_end));
        reset_keys();
        curr_song_index += 1;
        saved_clock = mils;
        next_state = sWAIT_FOR_KEY_TESTING;
      } else {
        next_state = sKEY_PRESSED_TESTING;
      }
        break;
    case sNO_KEY_PRESSED_TESTING: // 9 
      if (curr_song_index <= (song_end - 1) and mils - saved_clock >=note_durations[curr_song_index]) { // 9-7
        WDT->CLEAR.reg = 0xA5;
        //display(curr_song_index + “/” + song_end);
        Serial.println("going back to 7 from 9"); 
        light_led(song_notes[curr_song_index], RED, 0);
        Serial.println(String(curr_song_index) + String(song_end));
        if (curr_song_index != (song_end - 1)) {
          display_curr_index(curr_song_index+2, song_end);
        }
        curr_song_index += 1;
        saved_clock = mils;
        next_state = sWAIT_FOR_KEY_TESTING;
      } else {
        next_state = sNO_KEY_PRESSED_TESTING;
      }
      break;
    case sGAME_OVER: // 10
      if (mils - saved_clock >= 10000) { // 10-1
        WDT->CLEAR.reg = 0xA5;
        display_message("TESTING OVER");
        Serial.println("TESTING MODE OVER");
        countdown = 3;
        saved_clock = mils;
        num_correct_keys = 0;
        next_state = sWAIT_FOR_MODE;
      } else {
        WDT->CLEAR.reg = 0xA5;
        next_state = sGAME_OVER;
      }
      break;
  } 
    return next_state;
}
