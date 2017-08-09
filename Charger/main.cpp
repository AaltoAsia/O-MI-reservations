// vim settings:
// vim: et ts=2 sw=2
// Add support for OTA***************************************
#include <ArduinoOTA.h>
// **********************************************************
#include "Config.h"
#include "MicroUtil.h"
#include "types.h"
#include "OMIProcessing.h"
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <stdlib.h>
#include <string.h>
#include "SoftwareSerial.h"

extern "C" {
#include "user_interface.h"
}


SoftwareSerial Son_off(PIN_SONOFF_RX,PIN_SONOFF_TX);





void reconnect();
bool modemConnect();
void sonoff(void);
void send_current_or_power(int value,char* whichdata);
TinyGsm modem(SerialAT);
TinyGsmClient client(modem);
DB database;

WebSocketsClient webSocket;

#define MESSAGE_INTERVAL 35000
#define HEARTBEAT_INTERVAL 28000
#define RESPONSE_BUFFER 5120

#define LidStatusName FString("LidStatus")

#define GetValueOfItem(name, object, value) \
                tmp = 0; tmpb = 0; \
                if (!(findInfoItem((object), (name), item, tmp) \
                            && findValue(item, (value), tmpb))) { \
                    D("[OMIreserve] Error: Cannot parse "); DS(name); \
                    continue; \
                }


class StringBuffer : public String {
public:
  StringBuffer() : String() {}
  inline StringBuffer & copy_cstr(const char *cstr, unsigned int length) {
    this->copy(cstr,length);
    return *this;
  }
};
const char* PoleSubscriptionRequest = "<omiEnvelope xmlns=\"http://www.opengroup.org/xsd/omi/1.0/\" version=\"1.0\" ttl=\"-1\"><read msgformat=\"odf\" interval=\"-1\" callback=\"0\"><msg><Objects xmlns=\"http://www.opengroup.org/xsd/odf/1.0/\"><Object><id>HWTEST</id><InfoItem name=\"LidStatus\"/></Object></Objects></msg></read></omiEnvelope>";



/*const char* PoleSubscriptionRequest = 
  "<omiEnvelope xmlns=\"http://www.opengroup.org/xsd/omi/1.0/\" version=\"1.0\" ttl=\"-1\">"
   "<read msgformat=\"odf\" interval=\"-1\" callback=\"0\">"
     "<msg>"
       "<Objects xmlns=\"http://www.opengroup.org/xsd/odf/1.0/\">"
         "<Object>"
           "<id>ParkingService</id>"
           "<Object>"
             "<id>ParkingFacilities</id>"
             "<Object>"
               "<id>DipoliParkingLot</id>"
               "<Object>"
                 "<id>ParkingSpaceTypes</id>"
                 "<Object>"
                   "<id>ElectricVehicleParkingSpace</id>"
                   "<Object>"
                     "<id>Spaces</id>"
                     "<Object>"
                       "<id>EVSpace2</id>"
                       "<InfoItem name=\"Available\"/>"
                       "<Object>"
                         "<id>Charger</id>"
                         "<InfoItem name=\"LidStatus\"/>"
                       "</Object>"
                     "</Object>"
                   "</Object>"
                 "</Object>"
               "</Object>"
             "</Object>"
           "</Object>"
         "</Object>"
       "</Objects>"
     "</msg>"
   "</read>"
  "</omiEnvelope>" ;*/

uint64_t messageTimestamp = 0;
uint64_t heartbeatTimestamp = 0;
bool isConnected = false;
bool isSubscribed = false;
bool isXml = false;



//send power or current to server function.
bool send_flag=false;
void send_current_or_power(int value,char* whichdata){
	
String  str;
char *p;
String const currentdata2="</value></InfoItem></Object></Objects></msg></write></omiEnvelope>,";//you must add "," end of currentdata2 xml text to seperate xml text.
String  currentdata;
char currentdata1[]="15";
itoa(value, currentdata1, 10);

if(whichdata=="CURRENT"){

currentdata="<omiEnvelope xmlns=\"http://www.opengroup.org/xsd/omi/1.0/\" version=\"1.0\" ttl=\"-1\"><write msgformat=\"odf\"><msg><Objects xmlns=\"http://www.opengroup.org/xsd/odf/1.0/\"><Object><id>HWTEST</id><InfoItem name=\"Current\"> <value>";


}
else if(whichdata=="POWER"){

currentdata="<omiEnvelope xmlns=\"http://www.opengroup.org/xsd/omi/1.0/\" version=\"1.0\" ttl=\"-1\"><write msgformat=\"odf\"><msg><Objects xmlns=\"http://www.opengroup.org/xsd/odf/1.0/\"><Object><id>HWTEST</id><InfoItem name=\"Power\"> <value>";


}

str =currentdata+currentdata1+currentdata2;


	p = strtok(&str[0], ",");
send_flag=true;
if(value<15000){
webSocket.sendTXT(p);
}	
send_flag=false;
}


StringBuffer responsePayload;



os_timer_t myTimer;
volatile unsigned int beat_index=0,sonoff_index=0;
bool tickOccured=true;
unsigned int data=0;
// start of timerCallback
void timerCallback(void *pArg) {
	ESP.wdtDisable();



if(isConnected&&isSubscribed){
	beat_index++;
	sonoff_index++;
/*	if(beat_index==28){
    digitalWrite(D0,!digitalRead(D0)); 
	DLN("Sending heartbeat");
    webSocket.sendTXT("");
    beat_index=0;
	}*/

	
		
			
		if(tickOccured==true){
	data=(unsigned int)Son_off.get_data(3)*256+(unsigned int)Son_off.get_data(4);
	DLN("Sending current_data");
    send_current_or_power(data,"CURRENT");	
	tickOccured=false;

}
else{
    data=(unsigned int)Son_off.get_data(1)*256+(unsigned int)Son_off.get_data(2);
	DLN("Sending power_data");

	send_current_or_power(data,"POWER");
	tickOccured=true;

}	
	
}
       
 ESP.wdtEnable(WDTO_8S);   
      
} // End of timerCallback


bool ota_flag=false;
/*void handleInterrupt() {
  digitalWrite(PIN_LOCK, !digitalRead(PIN_LOCK));     // Initialize the LED_BUILTIN pin as an output
   

}*/



void user_init(void) {


      os_timer_setfn(&myTimer, timerCallback, NULL);
      os_timer_arm(&myTimer, 5000, true);//set timer for every 5sec
} // End of user_init

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {

if(ota_flag!=true){


    switch(type) {
        case WStype_DISCONNECTED:
            DFORMAT("[WSc] Disconnected!\n\r");
            isConnected = false;
           
           reconnect();
       
            break;
        case WStype_CONNECTED:
            {
                DFORMAT("[WSc] Connected to url: %s\n\r",  payload);
                isConnected = true;
            
                DLN("Sending Subscription");
                isSubscribed = false;
                user_init();//timer config for sending power/current data
			    
            }
            break;
        case WStype_TEXT:
            if (length > 0) {
              DFORMAT("[WSc] get text: %s\n\r", payload);
              isXml = true;
              responsePayload.copy_cstr((const char *) payload, length);
            
            }
            break;
        case WStype_BIN:
            DFORMAT("[WSc] get binary length: %u\n\r", length);
            hexdump(payload, length);

            // send data to server
            // webSocket.sendBIN(payload, length);
            break;
    }
}
}



void setup() {
 
 responsePayload.reserve(RESPONSE_BUFFER);
 noInterrupts();



  // Set console baud rate
  DEBUG_PORT.begin(115200);
  //SerialD;
  delay(10);

  // Set GSM module baud rate
  SerialAT.begin(115200);
  SerialAT.swap();
  delay(3000);
//sonooff serial 
  Son_off.begin(115200);
  Son_off.enableRx(true);
   delay(10);
  // PINS
 pinMode(PIN_LOCK, OUTPUT);
  digitalWrite(PIN_LOCK, LOW);
  pinMode(PIN_GSM_PWR, OUTPUT);
  digitalWrite(PIN_GSM_PWR, LOW);

pinMode(D0, OUTPUT);
digitalWrite(D0, LOW);

 //OTA update config
  ArduinoOTA.onError([](ota_error_t error) { ESP.restart(); });
  ArduinoOTA.begin();  /* setup the OTA server */
   ArduinoOTA.onStart([]() {
     digitalWrite(PIN_LOCK, HIGH);
  });
  ArduinoOTA.onEnd([]() {
    digitalWrite(PIN_LOCK, LOW);
  });
   ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
     digitalWrite(PIN_LOCK, HIGH);
  });
  // BOOT GSM
  delay(50);
  D("PWR");
  digitalWrite(PIN_GSM_PWR, HIGH);
  D(".HIGH");
  delay(3200);
  D("DONE");
  digitalWrite(PIN_GSM_PWR, LOW);
  D(".LOW");
  delay(300);
  DLN(".INIT...");

interrupts();
  modem.init();
	yield();    
  modemConnect();
 
  
  //  pinMode(D2, INPUT_PULLUP);
 // attachInterrupt(digitalPinToInterrupt(D2), handleInterrupt, RISING);
}

bool modemConnect() { // TODO: clean, move to websocket library?
  
  D("Waiting for network...");
  if (!modem.waitForNetwork()) {
    DLN(" fail, retry after 5 secs");
    delay(5000);
    return false;
  }
  DLN(" OK");

  D("Connecting to ");
  DSLN(apn);
  String ip;
  if (!modem.gprsConnect(apn, user, pass, ip)) {
    DLN(" fail, retry after 5 secs");
    delay(5000);
    return false;
  }
  D(" OK, Own IP address: ");
  DSLN(ip);

  // Commented: Connect instead in websocket library?
  D("Connecting to ");
  D(OMI_HOST);
  if (!client.connect(OMI_HOST, 80)) {
    DLN(" fail, retry after 5 secs");
    delay(5000);
    return false;
  }
  DLN(" CONNECT OK ");

  webSocket.onEvent(webSocketEvent);
  yield();
  webSocket.begin(&client, OMI_HOST, 80, OMI_PATH, "omi");
  yield();
  webSocket.connectedCb();
}


void reconnect(void){
  DLN("Initializing modem...");
  modem.init();
   
  modemConnect();
    yield();
}


void loop() {
  
  // if not connected try to connect
  if (!client.connected()) {
    //if (!reconnect()) return;
    reconnect();
    
  }



  for (int wait = 0; wait < 15 && !isConnected; wait++) {
    DLN("Waiting for connection.");
    auto time = millis() + 5000;
    while (millis() < time && !isConnected) {
      webSocket.loop();
    }
    yield();
  }


 
  while (isConnected) {

 while(ota_flag==true&&send_flag==false){//is there OTA UPDATE? 
  DLN("======OTA UPDATE==== ");
 //	noInterrupts();
 //   client.stop();
   // ESP.wdtDisable();
   // webSocket.disconnect();
  
    ArduinoOTA.handle();// Add support for OTA***************************************
  
    yield();
    //ESP.wdtEnable(WDTO_8S); 
    } 
    
if(ota_flag!=true){

   webSocket.loop();
      yield();
    
    yield();

   if (isXml) {
    		 
      isXml = false;
      String value;
      String item;
      unsigned tmp = 0, tmpb = 0;
      //GetValueOfItem(LidStatusName, responsePayload, value)
      if (findInfoItem(responsePayload, LidStatusName, item, tmp)) {
        if (findValue(item, value, tmpb)) {
        
          DFORMAT("Result: %s", value.c_str());
         
		 // digitalWrite(PIN_LOCK, HIGH);
         // delay(2000);
          //digitalWrite(PIN_LOCK, LOW);
           digitalWrite(PIN_LOCK, !digitalRead(PIN_LOCK));
		   if(value=="update"){//do we get update request from server?
		   		os_timer_disarm(&myTimer);
		   	ota_flag=true;
		   	 digitalWrite(PIN_LOCK, !digitalRead(PIN_LOCK));
        //	digitalWrite(PIN_LOCK, HIGH);
        //delay(2000);
        //digitalWrite(PIN_LOCK, LOW);
        	
		}
		else if(value=="open"){
		digitalWrite(PIN_LOCK, HIGH);
        delay(2000);
        digitalWrite(PIN_LOCK, LOW);
			
		}
        }
       
        
      }

    }
    
    //DFORMAT("RAM Memory left: %d\r\n", ESP.getFreeHeap());

 if (! isSubscribed) {
    
      webSocket.sendTXT(PoleSubscriptionRequest);
      isSubscribed = true; // TODO: Check response success
      
    }
}


  }



      yield();
}








