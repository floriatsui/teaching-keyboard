#include <LiquidCrystal.h>

/*
 * Initialize LCD based on Lab 5 schematic
 */
const int rs = 0, en = 1, d4 = 2, d5 = 3, d6 = 4, d7 = 5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


void clean_lcd() {
  lcd.clear();
  lcd.setCursor(0, 0); 
}

/*
 * Display message desired
 */
void display_message(String message) {
  Serial.println("executing display message"); 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(message);
  delay(500); 
}
