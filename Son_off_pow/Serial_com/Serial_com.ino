
#define LED 13
#define RELAY 12
#define BUTTON 14
#define START 0xAA
#define CURRENT_VALUE 0x00
#define POWER_VALUE 0x00
#define RELAY_ON 0x0F
#define RELAY_OFF 0x03
#define STOP 0xAA
volatile unsigned int index_senddata=0,index_measuring=0;
bool relay_status=0;
extern "C" {
#include "user_interface.h"
}
#include "power.h"
ESP8266PowerClass power_dev;
os_timer_t myTimer;
unsigned int power=0,current=0,voltage=0;
////************************SENDING TWO BYTES CONVERSION BLOCK************************////
  union U16_
  {
     unsigned int word;      // For accessing the whole 16-bit unsigned int
     unsigned char byte[2];  // For accessing 16-bits as individual bytes
  };
     void WriteMyUART_Bytes (unsigned int MyInWord)
{
union U16_ Int2Bytes;

   Int2Bytes.word = MyInWord;

          //  Serial.write(0xDC);
            Serial.write(Int2Bytes.byte[1]);       //high byte
            Serial.write(Int2Bytes.byte[0]);        //low byte

}     
////***********************************************************************************////
bool tickOccured;
////**********************TIMER FUNCTION FOR SENDING DATA TO ESP8266******************////
// start of timerCallback 
void timerCallback(void *pArg) {
index_senddata++;
 index_measuring++;   
   if(index_measuring==5){
     power=(int)power_dev.getPower();
     //current=(int)power_dev.getCurrent();
      //power=5000;
        //current=8900;
      if((int)power_dev.getPower()!=0){
      
       current=(int)power_dev.getCurrent();
       if(current>13000){//relay status
        
        }
      }
   
      index_measuring=0;
    
  }
 if(index_senddata==8){
     Serial.write(0xAA);
     WriteMyUART_Bytes(power);
     WriteMyUART_Bytes(current);
     Serial.write(relay_status);
     Serial.write(0xAA);
     
      index_senddata=0;
    
  }
}



void timer_init(void) {


      os_timer_setfn(&myTimer, timerCallback, NULL);


      os_timer_arm(&myTimer, 1000, true);
} // End of user_init

////***********************************************************************************////




void power_init(void){
  power_dev.setPowerParam(12.65801022,0.0);
    power_dev.setCurrentParam(19.52,-85.9);
    power_dev.setVoltageParam(0.45039823,0.0);
    power_dev.enableMeasurePower();
    power_dev.selectMeasureCurrentOrVoltage(CURRENT);
    power_dev.startMeasure();
}
byte data[5]={START,RELAY_ON,0x0D,0x0D,STOP};
int flag=1;
    



void setup() {
  Serial.begin(115200);     // Initialize the LED_BUILTIN pin as an output
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);  // Turn the LED off by making the voltage HIGH
  
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, HIGH);  // RELAY_ON
  relay_status=1;
  delay(3000);                      // Wait for two seconds (to demonstrate the active low LED)
 

 power_init();//power measuring config
 timer_init();//timer config for every sec
}

// the loop function runs over and over again forever
void loop() {

}



