// #include "final_fsm.h"
/*
 * Initialize LCD based on Lab 5 schematic
 */
const int rs = 0, en = 1, d4 = 2, d5 = 3, d6 = 4, d7 = 5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


/*
 * Initialize system: set up LCD
 */
void initialize_system() {
  last_key = -1;
  num_keys = 0;
  lcd.begin(16,2);
  toggleSwitch.setDebounceTime(200);
}

/*
 * Display "CALIBRATING" as a scroll on the LCD
 * Display capacitive sensor readings from pins 7-9 in serial monitor
 */
void calibrate() {
  int i = 0;
  bool left = true;
  lcd.setCursor(0,0);
  lcd.print("CALIBRATING...");
  int scroll_len = ((String)"CALIBRATING...").length();
  delay(400);
  while(true) {
    Serial.println("Capacitive sensing:");
    Serial.print("7:  ");
    Serial.println(G.capacitiveSensorRaw(10));
    Serial.print("8:  ");
    Serial.println(A.capacitiveSensorRaw(10));
    Serial.print("9:  ");
    Serial.println(B.capacitiveSensorRaw(10));
    Serial.println();
    if (i == scroll_len) {
      i = 0;
      lcd.clear();
      if (left) {
        lcd.setCursor(16-scroll_len, 1);
        lcd.print("CALIBRATING...");
      } else {
        lcd.setCursor(0, 0);
        lcd.print("CALIBRATING...");
      }
      left = !left;
    } else {
      if (left) {
        lcd.scrollDisplayLeft();
      } else {
        lcd.scrollDisplayRight();
      }
    }
    i += 1;
    delay(300);
  }
}

/*
 * Display which sensor has been pressed on the keyboard, assuming labels[] and thresholds[]
 * have been set up correctly
 */
void test_calibration() {
  String labels[4];
  // A hack, because enum "orientation" defines values from 0 to 2
  labels[0] = "G pressed";
  labels[1] = "A pressed";
  labels[2] = "B pressed";

  while(true) {
    lcd.clear();
    // A hack, because enum "orientation" defines values from 0 to 2
    for(int i = 0; i < 3; i++) {
      int cap_reading;
      switch(cap_sensors[i]) {
        case 8:
          cap_reading = G.capacitiveSensorRaw(10);
          break;
        case 9:
          cap_reading = A.capacitiveSensorRaw(10);
          break;
        case 10:
          cap_reading = B.capacitiveSensorRaw(10);
          break;
        default:
          break;        
      }
      if (cap_reading > thresholds[i]) {
        lcd.setCursor(0,0);
        //lcd.print(labels[i]);
        Serial1.write("hello");
        //Serial1.write(labels[i]);
      }
    }
    delay(50);
  }
}

/*
 * Read in keyboard inputs and update num_keys and last_key
 * We only increase "num_keys" if the current key pressed is not the same as the previous key pressed
 */
void update_inputs() {
  // A hack, because enum "orientation" defines values from 0 to 3
  for(int i = 0; i < 3; i++) {
    int cap_reading;
    switch(cap_sensors[i]) {
      case 8:
        cap_reading = G.capacitiveSensorRaw(10);
        break;
      case 9:
        cap_reading = A.capacitiveSensorRaw(10);
        break;
      case 10:
        cap_reading = B.capacitiveSensorRaw(10);
        break;
      default:
        break;        
    }
    if (cap_reading > thresholds[i]) {
      num_keys = 1;
      last_key = keys[i];
    }
  }
}

/*
 * Read in mode switch input and set the current mode accordingly
 */
void set_mode() {

  //noInterrupts();
  toggleSwitch.loop();
  
  int state = toggleSwitch.getState();

  if (state == HIGH){
    //Serial1.write("Testing Mode*");
    // lcd.print("Testing mode!");
    curr_mode = TESTING;
    return;
  }
    
  else {
    //Serial1.write("Learning Mode*");
    // lcd.print("Learning mode!");
    curr_mode = LEARNING;
    return;
  }
  delay(100);
  //interrupts();
}

void update_mode() {
  noInterrupts(); 
  Serial1.write("switching modes!*");
  Serial.println("switching modes!*");

  if (curr_mode == TESTING) {
    Serial1.write("Learning Mode*");
    Serial.println("Learning Mode*");
    curr_mode = LEARNING;
  } else {
    curr_mode = TESTING;
    Serial1.write("Testing Mode*");
    Serial.println("Testing Mode*");
  }

  reset_keys();
  
  CURRENT_STATE = update_fsm(sWAIT_FOR_MODE, millis(), num_keys, last_key, curr_mode);

  light_led(NOTE_A5, RED, 0);
  light_led(NOTE_A5, GREEN, 0);
  light_led(NOTE_G5, RED, 0);
  light_led(NOTE_G5, GREEN, 0);
  light_led(NOTE_B5, RED, 0);
  light_led(NOTE_B5, GREEN, 0);
  delay(10000);
  interrupts();
}

/*
 * Reset num_keys to 0 and last_key to an undefined value
 */
void reset_keys() {
  last_key = -1;
  num_keys = 0;
}

/*
 * Display progression through song on the LCD
 */
void display_curr_index(int curr_index, int total_notes) {
  /*lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("On note: ");
  lcd.print(curr_index + 1);
  lcd.setCursor(0,1);
  lcd.print(" / ");
  lcd.print(total_notes + 1);*/

  //"On note: " + (curr_index + 1) + " / " + (total_notes + 1));
  String message = "On note: " + String(curr_index) + " / " + String((total_notes)) + "*";
  char buf[256];
  message.toCharArray(buf, 256);
  Serial1.write(buf);
}

/*
 * Display message desired
 */
void display_message(String message) {
  //lcd.clear();
  message += "*";
  //Serial1.write(message + "*");
  char buf[256];
  message.toCharArray(buf, 256);
  Serial1.write(buf);
  delay(1000);
  //lcd.print(message);
}

/*
 * Light the desired LED with specified color, 
 * duration and frequency
 */
void light_led(int curr_note, color c, int frequency) {

  int thisNote;
  
  if (curr_note == NOTE_G5){
    if (c == GREEN){
      thisNote = Ggreen;
    } else {
      thisNote = Gred;
    }
  } else if (curr_note == NOTE_A5){
    if (c == GREEN){
      thisNote = Agreen;
    } else {
      thisNote = Ared;
    }
  } else {
    if (c == GREEN){
      thisNote = Bgreen;
    } else {
      thisNote = Bred;
    }
  }
  Serial.println(thisNote); 
  analogWrite(thisNote, frequency);
}


void play_demo_note(int curr_note, int duration) {
  // move duration calculation to its own helper if necessary
  Serial.println("play demo note"); 
  // first argument is the pin 
  Serial.println(duration);
  tone(12, curr_note, duration);
  delay(duration); 
  noTone(12); 
}

/*
 * Play the desired note for the specified duration
 */
void play_note(int curr_note, int duration, int start_of_note, int curr_time) {
  // move duration calculation to its own helper if necessary
  Serial.println("play note"); 
  Serial.println(curr_note);
  int offset = curr_time - start_of_note; 
  int play_duration = duration - offset; 
  Serial.println(play_duration); 
  // first argument is the pin 
  tone(12, curr_note, play_duration);
  delay(play_duration);
  noTone(12); 
  Serial.println("end of play note"); 

  //testing watchdog
  //delay(20000);
}

void WDT_Handler() {
  // Clear interrupt register flag
  WDT->INTFLAG.reg = 1;
  
  // Warn user that a watchdog reset may happen
  Serial.println("reset may happen");
}
