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
#define BUTTON 14
#define START 0xAA
#define CURRENT_VALUE 0x00
#define POWER_VALUE 0x00
#define RELAY_ON 0x0F
#define RELAY_OFF 0x03
#define STOP 0xAA


byte data[5]={START,RELAY_ON,0x0D,0x0D,STOP};
int flag=1;
    



void setup() {
  Serial.begin(115200);     // Initialize the LED_BUILTIN pin as an output
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);  // Turn the LED off by making the voltage HIGH
  pinMode(BUTTON, INPUT);
  digitalWrite(BUTTON, LOW);  // Turn the LED off by making the voltage HIGH
  
  pinMode(RELAY, OUTPUT);
  
  delay(3000);                      // Wait for two seconds (to demonstrate the active low LED)
  Serial.print("Hello World!\n");
}

// the loop function runs over and over again forever
void loop() {
/*if(digitalRead(BUTTON)==HIGH){
 // digitalWrite(LED_BUILTIN, HIGH);// Ledi yak
  digitalWrite(LED, LOW);

  if(flag==1){
    flag=0;
    data[1]=RELAY_ON;
    }else{
      flag=1;
      data[1]=RELAY_OFF;
      }
 for(int i=0;i<5;i++){
   Serial.write(data[i]);
  }

    
  
  delay(3000);
  }
  else{
     digitalWrite(LED, HIGH);
  //digitalWrite(LED_BUILTIN, LOW);// Ledi yak

  }*/
digitalWrite(LED, HIGH);
 digitalWrite(RELAY, HIGH);
 Serial.print("turn on!\n");
delay(2000);
Serial.print("turn off!\n");
  digitalWrite(LED, LOW);
   digitalWrite(RELAY, LOW);
delay(1000);
  


}



