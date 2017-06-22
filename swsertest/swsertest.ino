
#include <SoftwareSerial.h>
//const byte interruptPin = D1;
SoftwareSerial swSer(D1,D2);
#define START 0xAA
#define CURRENT_VALUE 0x00
#define POWER_VALUE 0x00
#define RELAY_ON 0x0F
#define RELAY_OFF 0x03
#define STOP 0xAA
int FLAG=0;
boolean stringComplete = false;
boolean interrupt = false;
//int  serIn;             // var that will hold the bytes-in read from the serialBuffer
//byte serInString[4];  // array that will hold the different bytes  100=100characters;
                        // -> you must state how long the array will be else it won't work.
//int  serInIndx  = 0;    // index of serInString[] in which to insert the next incoming byte
//int  serOutIndx = 0;    // index of the outgoing serInString[] array;

volatile int i=0;
/*void read_MultipleBytes () {
    int sb;  
   sb = swSer.read();   
     swSer.write(sb);
     swSer.print("WELCOME!\n");
    if(swSer.available()) { 
    swSer.print("WELCOME!\n");
       //Serial.print("reading Serial String: ");     //optional confirmation
       while (swSer.available()){ 
                     
          serInString[serInIndx] = sb;
          serInIndx++;
                                 //optional confirmation
          
        if(serInIndx==5){//GET 5 BYTE DATA 
          if(serInString[0]==START&&serInString[4]==STOP){
            stringComplete=true;
           serInIndx=0;
            interrupt=false;
            }
           
          }
         
       }
       
    }  
    
}*/

void setup() {
  Serial1.begin(115200);
  delay(10);
   noInterrupts();
  // Set GSM module baud rate
  Serial.begin(115200);
//  Serial.swap();-
 //attachInterrupt(D7, serialInterrupt, CHANGE);
  swSer.begin(115200);
  swSer.enableRx(true);
pinMode(LED_BUILTIN, OUTPUT);
digitalWrite(LED_BUILTIN, LOW);
pinMode(D0, OUTPUT);
digitalWrite(D0, LOW);
//pinMode(D1, INPUT_PULLUP);
//attachInterrupt(digitalPinToInterrupt(D1), handleInterrupt,RISING);
 interrupts();
}

void loop() {
 digitalWrite(LED_BUILTIN, HIGH);
     delay(500);
digitalWrite(LED_BUILTIN, LOW);
     delay(500);
}




/*void handleInterrupt(){
  digitalWrite(LED_BUILTIN, HIGH);
   if(Serial.available()){
     int sb;  
   while (Serial.available()){
   serInString[i] = Serial.read(); 
   i++;
   if(i==4){
  i=0;
 FLAG=1;
  }
    //Serial.print(sb);
   }
    }

 // delay(200);
  digitalWrite(LED_BUILTIN, LOW);
  
  }*/











 /*if (stringComplete) {
    
    swSer.print("WELCOME!\n");
    if (serInString[1] == 0x0F) {
        digitalWrite(LED_BUILTIN, LOW);   // Turn the LED ON 
        
        swSer.print("LED ON!\n");
     
    }
    else if (serInString[1] == 0x03) {
        digitalWrite(LED_BUILTIN, LOW);  // Turn the LED on by making the voltage HIGH
      

    }
    stringComplete=false;
  }
                                    
  delay(50);                      // Wait for a second*/

