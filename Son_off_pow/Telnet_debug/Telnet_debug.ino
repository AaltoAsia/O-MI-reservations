//#define TELNET_DEBUG
#define SERIAL_COM

extern "C" {
#include "user_interface.h"
}
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <ESP8266mDNS.h>
#include "power.h"
ESP8266PowerClass power_dev;
// Remote debug over telnet - not recommended for production, only for development
#define LED 13
#define RELAY 12
#include "RemoteDebug.h"       

RemoteDebug Debug;
float a=0;
// SSID and password

const char* ssid = "aalto open";
const char* password = "";

// Host mDNS

#define HOST_NAME "remotedebug-sample"

// Time

uint32_t mLastTime = 0;
uint32_t mTimeSeconds = 0;

// Buildin Led ON ?

boolean mLedON = false;

////// Setup


////**********************TIMER FUNCTION FOR SENDING DATA TO ESP8266******************////
os_timer_t myTimer;
unsigned int power=0,current=0,voltage=0;
bool relay_status=0;

volatile unsigned int index_senddata=0,index_measuring=0;
// start of timerCallback
void timerCallback(void *pArg) {
index_senddata++;
 index_measuring++;   

 if(index_measuring==5){
     power=(int)power_dev.getPower();
     if((int)power_dev.getPower()!=0){
       current=(int)power_dev.getCurrent();
       if(power<20){
   relay_status=0;
        }
        else{
          relay_status=1;
          }
      }
    
     // power=5000;
      //current=8900;
    //  voltage=(int)power_dev.getVoltage();
      index_measuring=0;
    
  }
 if(index_senddata==8){
      #ifdef TELNET_DEBUG
      Debug.println("=============POWER==============");
      Debug.printf("POWER: %u\n",power);
      Debug.println("=====================================");
      
      Debug.println("============CURRENT==================");
      Debug.printf("CURRENT: %u\n",current);
      Debug.println("=====================================");
      #endif
      
     #ifdef SERIAL_COM
     Serial.write(0xAA);
     WriteMyUART_Bytes(power);
     WriteMyUART_Bytes(current);
     Serial.write(relay_status);
     Serial.write(0xAA);
     #endif
      index_senddata=0;
    
  }
    
 


       
  } // End of timerCallback

void timer_init(void) {


      os_timer_setfn(&myTimer, timerCallback, NULL);


      os_timer_arm(&myTimer, 1000, true);
} // End of user_init

////**********************TIMER FUNCTION FOR SENDING DATA TO ESP8266******************////

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

           
            Serial.write(Int2Bytes.byte[1]);       //high byte
            Serial.write(Int2Bytes.byte[0]);        //low byte
            

}     
////************************SENDING TWO BYTES CONVERSION BLOCK************************////


//SONOFF CONFIG 
void power_init(void){
   power_dev.setPowerParam(12.98801022,0.0);
    power_dev.setCurrentParam(19.52,-85.9);
    power_dev.setVoltageParam(0.45039823,0.0);
    power_dev.enableMeasurePower();
    power_dev.selectMeasureCurrentOrVoltage(CURRENT);
    power_dev.startMeasure();
}

//REMOTE DEBUG CONFIG
void debug_init(){
    WiFi.begin(ssid, password);
    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
     // Serial.print(".");
    }
    // Register host name in WiFi and mDNS

    String hostNameWifi = HOST_NAME;
    hostNameWifi.concat(".local");
    WiFi.hostname(hostNameWifi);

    MDNS.addService("telnet", "tcp", 23);
    Debug.begin(HOST_NAME); // Initiaze the telnet server

    Debug.setResetCmdEnabled(true); // Enable the reset command

  }
  
void setup() {

    // Initialize the Serial (educattional use only, not need in production)

    Serial.begin(9600);

    // Buildin led off ESP8266

   pinMode(LED, OUTPUT);
   digitalWrite(LED, LOW);
   pinMode(RELAY, OUTPUT);
   digitalWrite(RELAY, HIGH);
    // WiFi connection
   relay_status=1;

 #ifdef TELNET_DEBUG
 debug_init();
 #endif 
 power_init();
 timer_init();//timer config for every sec
}

void loop()
{ // Serial.println(WiFi.localIP());
    // Each second
 
    
        // Time
   #ifdef TELNET_DEBUG
        mLastTime = millis();

        mTimeSeconds++;

        // Blink the led

        mLedON = !mLedON;
       digitalWrite(LED, (mLedON)?LOW:HIGH);

        // Debug the time (verbose level)

        if (Debug.isActive(Debug.VERBOSE)) {
            Debug.printf("* Time: %u seconds (VERBOSE)\n", mTimeSeconds);
        }

      

    // Remote debug over telnet

    Debug.handle();

    // Give a time for ESP8266

    yield();
   #endif

}

/////////// End
