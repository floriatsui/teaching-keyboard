#include <CapacitiveSensor.h>
#include "pitches.h"

char keys[3];
int key_threshold[3];
int key_notes[3]; 
const int PB_PIN = 10;

typedef enum {
  LEFT = 0,
  MIDDLE = 1,
  RIGHT = 2,
} note;


CapacitiveSensor s7 = CapacitiveSensor(11, 7);
CapacitiveSensor s8 = CapacitiveSensor(11, 8);
CapacitiveSensor s9 = CapacitiveSensor(11, 9);

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // Set piezo speaker as output:
  PORT->Group[PORTB].DIRSET.reg |= (1<<PB_PIN);
   
  // put your setup code here, to run once:
  keys[LEFT] = 9;
  keys[MIDDLE] = 8;
  keys[RIGHT] = 7;

  key_threshold[LEFT] = 1700;
  key_threshold[MIDDLE] = 2000;
  key_threshold[RIGHT] = 3200;

  key_notes[LEFT] = NOTE_G5; 
  key_notes[MIDDLE] = NOTE_A5;
  key_notes[RIGHT] = NOTE_B5;

  Serial.println("Initialized!");
}

void play_note(int i) {
  Serial.println("play note"); 
  // toggle output pin to play note
  // PORT->Group[PORTB].OUTTGL.reg |= (1<<10);
  tone(4, key_notes[i], 1000/4);
}

void update_inputs() {
  // A hack, because enum "orientation" defines values from 0 to 3
  for(int i = 0; i < 3; i++) {
    int cap_reading;
    switch(keys[i]) {
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
    Serial.println((note)keys[i]); 
    Serial.println(cap_reading); 
    if (cap_reading > key_threshold[i]) {
      // last_note_pressed = (note) i;
      play_note(i); 
    }
  }
}

void loop() {
  Serial.println("calling loop");
  // put your main code here, to run repeatedly:
  update_inputs(); 
}
