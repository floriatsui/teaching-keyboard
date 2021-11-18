/*
 * Initialize LCD based on Lab 5 schematic
 */
const int rs = 0, en = 1, d4 = 2, d5 = 3, d6 = 4, d7 = 5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

/*
 * Initialize system: set up LCD
 */
void initialize_system() {
  last_key = (note) -1;
  num_keys = 0;
  lcd.begin(16,2);
  toggleSwitch.setDebounceTime(50);
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
    Serial.println(s7.capacitiveSensorRaw(10));
    Serial.print("8:  ");
    Serial.println(s8.capacitiveSensorRaw(10));
    Serial.print("9:  ");
    Serial.println(s9.capacitiveSensorRaw(10));
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
  labels[G] = "G pressed";
  labels[A] = "A pressed";
  labels[B] = "B pressed";

  while(true) {
    lcd.clear();
    // A hack, because enum "orientation" defines values from 0 to 2
    for(int i = 0; i < 3; i++) {
      int cap_reading;
      switch(cap_sensors[i]) {
        case 7:
          cap_reading = s7.capacitiveSensorRaw(10);
          break;
        case 8:
          cap_reading = s8.capacitiveSensorRaw(10);
          break;
        case 9:
          cap_reading = s9.capacitiveSensorRaw(10);
          break;
        default:
          break;        
      }
      if (cap_reading > thresholds[i]) {
        lcd.setCursor(0,0);
        lcd.print(labels[i]);
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
      case 7:
        cap_reading = s7.capacitiveSensorRaw(10);
        break;
      case 8:
        cap_reading = s8.capacitiveSensorRaw(10);
        break;
      case 9:
        cap_reading = s9.capacitiveSensorRaw(10);
        break;
      default:
        break;        
    }
    if (cap_reading > thresholds[i]) {
      if ((int) last_key != i) {
        num_keys += 1;
      }
      last_key = (note) i;
    }
  }
}

/*
 * Read in mode switch input and set the current mode accordingly
 */
void set_mode() {
  toggleSwitch.loop();
  
  int state = toggleSwitch.getState();

  if (state == HIGH){
    Serial.println("Testing Mode");
    // lcd.print("Testing mode!");
    curr_mode = TESTING;
  }
    
  else {
    Serial.println("Learning Mode");
    // lcd.print("Learning mode!");
    curr_mode = LEARNING;
  }
}

/*
 * Reset num_keys to 0 and last_key to an undefined value
 */
void reset_keys() {
  last_key = (note) -1;
  num_keys = 0;
}

/*
 * Display progression through song on the LCD
 */
void display_curr_index(int curr_index, int total_notes) {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("On note: ");
  lcd.print(curr_index + 1);
  lcd.setCursor(0,1);
  lcd.print(" / ");
  lcd.print(total_notes + 1);
}

/*
 * Display message desired
 */
void display_message(String message) {
  lcd.clear();
  lcd.print(message);
}

/*
 * Light the desired LED with specified color, 
 * duration and frequency
 */
void light_led(note curr_note, int duration, String color, int frequency) {

  int thisNote;
  
  if (note == G){
    if (color == "GREEN"){
      thisNote = Ggreen;
    } else {
      thisNote = Gred;
    }
  } else if (note == A){
    if (color == "GREEN"){
      thisNote = Agreen;
    } else {
      thisNote = Ared;
    }
  } else {
    if (color == "GREEN"){
      thisNote = Bgreen;
    } else {
      thisNote = Bred;
    }
  }
  
}

/*
 * Dim the desired LED over the duration specified
 */
void dim_led(note curr_note, int duration, String color) {
  // note -- will be one of G, A, or B
  // duration -- milliseconds
  // color-- "GREEN" or "RED"

  // PWM is 0 to 255, we want to dim over the duration of the note
  int pwm = 255;
  int deltaPWM = 255/10;
  int delayTime = duration / 10;
  int thisNote;
  
  if (note == G){
    if (color == "GREEN"){
      thisNote = Ggreen;
    } else {
      thisNote = Gred;
    }
  } else if (note == A){
    if (color == "GREEN"){
      thisNote = Agreen;
    } else {
      thisNote = Ared;
    }
  } else {
    if (color == "GREEN"){
      thisNote = Bgreen;
    } else {
      thisNote = Bred;
    }
  }

  while (pwm >= 0){
    analogWrite(thisNote, pwm);
    delay(delayTime);
    pwm -= deltaPWM;
  }

  
}

/*
 * Play the desired note for the specified duration
 */
void play_note(note curr_note, int duration) {
  
}
