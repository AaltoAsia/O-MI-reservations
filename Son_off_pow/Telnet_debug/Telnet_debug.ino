
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

const char* ssid = "NETGEAR91";
const char* password = "melodictree981";

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
       if(current>13000){
        
        }
      }
    
     // power=5000;
      //current=8900;
    //  voltage=(int)power_dev.getVoltage();
      index_measuring=0;
    
  }
 if(index_senddata==8){
  Debug.println("=============POWER==============");
    //  WriteMyUART_Bytes(power);
      //Debug.printf("\n");
     Debug.printf("POWER: %u\n",power);
     Debug.println("=====================================");
      Debug.println("============CURRENT==================");
      //WriteMyUART_Bytes(current);
     //  Debug.printf("\n");
      Debug.printf("CURRENT: %u\n",current);
      Debug.println("=====================================");
       Serial.write(0xAA);
     WriteMyUART_Bytes(power);
     WriteMyUART_Bytes(current);
     Serial.write(relay_status);
     Serial.write(0xAA);
     
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

           // Debug.print(0xDC);
            Debug.print(Int2Bytes.byte[1]);       //high byte
           //  Debug.printf("\n");
            Debug.print(Int2Bytes.byte[0]);        //low byte
            // Debug.printf("\n");

}     
////************************SENDING TWO BYTES CONVERSION BLOCK************************////

void setup() {

    // Initialize the Serial (educattional use only, not need in production)

    Serial.begin(115200);

    // Buildin led off ESP8266

   pinMode(LED, OUTPUT);
   digitalWrite(LED, LOW);
 pinMode(RELAY, OUTPUT);
   digitalWrite(RELAY, HIGH);
    // WiFi connection
 relay_status=1;
    WiFi.begin(ssid, password);
  //  Serial.println("");

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
     // Serial.print(".");
    }

 /*   Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());*/

    // Register host name in WiFi and mDNS

    String hostNameWifi = HOST_NAME;
    hostNameWifi.concat(".local");

    WiFi.hostname(hostNameWifi);

    if (MDNS.begin(HOST_NAME)) {
      //  Serial.print("* MDNS responder started. Hostname -> ");
       // Serial.println(HOST_NAME);
    }

    MDNS.addService("telnet", "tcp", 23);

    // Initialize the telnet server of RemoteDebug

    Debug.begin(HOST_NAME); // Initiaze the telnet server

    Debug.setResetCmdEnabled(true); // Enable the reset command

    //Debug.showTime(true); // To show time

    // Debug.showProfiler(true); // To show profiler - time between messages of Debug
                              // Good to "begin ...." and "end ...." messages

    // This sample (serial -> educattional use only, not need in production)

   //Serial.println("* Arduino RemoteDebug Library");
   // Serial.println("*");
   // Serial.print("* WiFI connected. IP address: ");
   // Serial.println(WiFi.localIP());
   // Serial.println("*");
   // Serial.println("* Please use the telnet client (telnet for Mac/Unix or putty and others for Windows)");
   // Serial.println("*");
   // Serial.println("* This sample will send messages of debug in all levels.");
   // Serial.println("*");
   // Serial.println("* Please try change debug level in telnet, to see how it works");
   // Serial.println("*");
     power_dev.setPowerParam(12.65801022,0.0);
    power_dev.setCurrentParam(19.52,-85.9);
    power_dev.setVoltageParam(0.45039823,0.0);
    power_dev.enableMeasurePower();
    power_dev.selectMeasureCurrentOrVoltage(CURRENT);
    power_dev.startMeasure();
   timer_init();//timer config for every sec
}

void loop()
{ // Serial.println(WiFi.localIP());
    // Each second
 
    
        // Time

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


}

/////////// End
