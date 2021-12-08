
int incomingByte = 0; 
String test_str = ""; 

void setup() {
  Serial.begin(9600);  // Initialize serial monitor
  while (!Serial);     // Wait for serial monitor to start (need this for USB serial)
  Serial1.begin(9600); // Initialize TX/RX communication (do not need to wait)
  display_message("Ready!");
}

void loop() {
  if (Serial1.available() > 0) {
    char data = (char)Serial1.read(); 
    
    // end of the data stream delimiter 
    if (data == '*') {
      Serial.println(test_str); 
      display_message(test_str); 
      test_str = "";  
      return; 
    }
    test_str += data; 
    clean_lcd(); 
  }
}
