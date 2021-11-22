#include <LiquidCrystal.h>
#include <ezButton.h>

/*
 * Initialize LCD based on Lab 5 schematic
 */
const int rs = 0, en = 1, d4 = 2, d5 = 3, d6 = 4, d7 = 5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
//int switch_pin = 7;
ezButton toggleSwitch(5);

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  while (!Serial);

  //lcd.begin(16,2);
  //pinMode(switch_pin, INPUT);
  toggleSwitch.setDebounceTime(50);
  
}

void loop() {

  //lcd.clear();
  toggleSwitch.loop();

  if (toggleSwitch.isPressed()){
    Serial.println("The switch: Testing -> Learning");
  }

  if (toggleSwitch.isReleased()){
    Serial.println("The switch: Learning -> Testing");
    
  }

  int state = toggleSwitch.getState();
  if (state == HIGH){
    Serial.println("Testing Mode");
    //lcd.print("Testing mode!");
  }
    
  else {
    Serial.println("Learning Mode");
    //lcd.print("Learning mode!");
  }

  delay(100);

}
