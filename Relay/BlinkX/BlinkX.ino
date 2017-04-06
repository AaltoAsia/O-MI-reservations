/*
 ESP8266 Blink by Simon Peter
 Blink the blue LED on the ESP-01 module
 This example code is in the public domain
 
 The blue LED on the ESP-01 module is connected to GPIO1 
 (which is also the TXD pin; so we cannot use Serial.print() at the same time)
 
 Note that this sketch uses LED_BUILTIN to find the pin with the internal LED
*/
#define LED 13
#define RELAY 12

void setup() {
  Serial.begin(115200);     // Initialize the LED_BUILTIN pin as an output
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);  // Turn the LED off by making the voltage HIGH

  pinMode(RELAY, OUTPUT);
  
  delay(3000);                      // Wait for two seconds (to demonstrate the active low LED)
  Serial.print("Hello World!\n");
}

// the loop function runs over and over again forever
void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    if (c == 'I') {
        digitalWrite(LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
        digitalWrite(RELAY, HIGH);
    }
    else if (c == 'O') {
        digitalWrite(LED, HIGH);  // Turn the LED off by making the voltage HIGH
        digitalWrite(RELAY, LOW);

    }
  }
                                    // but actually the LED is on; this is because 
                                    // it is acive low on the ESP-01)
  delay(50);                      // Wait for a second

}
