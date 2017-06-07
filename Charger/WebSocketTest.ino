// vim settings:
// vim: et ts=2 sw=2

#include "Config.h"
#include "MicroUtil.h"
#include "types.h"
#include <ESP8266WiFi.h>
#include <Hash.h>



TinyGsm modem(SerialAT);
TinyGsmClient client(modem);
DB database;

WebSocketsClient webSocket;

#define MESSAGE_INTERVAL 35000
#define HEARTBEAT_INTERVAL 28000

uint64_t messageTimestamp = 0;
uint64_t heartbeatTimestamp = 0;
bool isConnected = false;
bool isSubscribed = false;
volatile int a;
WStype_t state;
const char* PoleSubscriptionRequest = 
"<omiEnvelope xmlns=\"http://www.opengroup.org/xsd/omi/1.0/\" version=\"1.0\" ttl=\"-1\">"
 "<read msgformat=\"odf\" interval=\"-1\" callback=\"0\">"
   "<msg>"
     "<Objects xmlns=\"http://www.opengroup.org/xsd/odf/1.0/\">"
       "<Object>"
         "<id>ParkingService</id>"
         "<Object>"
           "<id>ParkingFacilities</id>"
           "<Object>"
             "<id>CSBuildingParkingLot</id>"
             "<Object>"
               "<id>ParkingSpaceTypes</id>"
               "<Object>"
                 "<id>ElectricVehicleParkingSpace</id>"
                 "<Object>"
                   "<id>Spaces</id>"
                   "<Object>"
                     "<id>EVSpace1</id>"
                     "<InfoItem name=\"LidStatus\"/>"
                     "<InfoItem name=\"Available\"/>"
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
"</omiEnvelope>" ;


void reconnect(void){
 DLN("Initializing modem...");
  // BOOT GSM
  pinMode(PIN_GSM_PWR, OUTPUT);
  D("PWR");
  digitalWrite(PIN_GSM_PWR, HIGH);
  D(".HIGH");
  delay(4000);
  D("DONE");
  digitalWrite(PIN_GSM_PWR, LOW);
  D(".LOW");
  delay(500);
  DLN(".INIT...");
  modem.init();
	    
  yield();
  webSocket.begin(&client, OMI_HOST, 80, OMI_PATH, "omi");
  yield();
  webSocket.connectedCb();

}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {


    switch(type) {
        case WStype_DISCONNECTED:
            DFORMAT("[WSc] Disconnected!\n\r");
            isConnected = false;
          //  state=type;
           reconnect();
       
            break;
        case WStype_CONNECTED:
            {
                DFORMAT("[WSc] Connected to url: %s\n\r",  payload);
                isConnected = true;
            //state=type;
                DLN("Sending Subscription");
                isSubscribed = false;
                a=1;
            }
            break;
        case WStype_TEXT:
            DFORMAT("[WSc] get text: %s\n\r", payload);

			// send message to server
			// webSocket.sendTXT("message here");
            break;
        case WStype_BIN:
            DFORMAT("[WSc] get binary length: %u\n\r", length);
            hexdump(payload, length);

            // send data to server
            // webSocket.sendBIN(payload, length);
            break;
    }

}




void setup() {

  // Set console baud rate
  DEBUG_PORT.begin(115200);
  //SerialD;
  delay(10);

  // Set GSM module baud rate
  SerialAT.begin(115200);
  SerialAT.swap();
  delay(5000);

  // Restart takes quite some time
  // To skip it, call init() instead of restart()
  DLN("Initializing modem...");
  // BOOT GSM
  pinMode(PIN_GSM_PWR, OUTPUT);
  D("PWR");
  digitalWrite(PIN_GSM_PWR, HIGH);
  D(".HIGH");
  delay(4000);
  D("DONE");
  digitalWrite(PIN_GSM_PWR, LOW);
  D(".LOW");
  delay(500);
  DLN(".INIT...");
  modem.init();
  //while (!modem.restart()){
  //  DLN("FAIL");
  //  delay(5000);
  //  DLN("Initializing modem...");
  //}

  DLN("Done");
  delay(1000);
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




void loop() {
    DFORMAT("==state==: %d\r\n",a);
  // if not connected try to connect
  if (!client.connected()) {
    if (!modemConnect()) return;
  }



  for (int wait = 0; wait < 15 && !isConnected; wait++) {
    DLN("Waiting for connection.");
    auto time = millis() + 5000;
    while (millis() < time && !isConnected) {
      webSocket.loop();
    }
  }




  while (isConnected) {
    webSocket.loop();
    //DFORMAT("RAM Memory left: %d\r\n", ESP.getFreeHeap());

    if (! isSubscribed) {
      webSocket.sendTXT(PoleSubscriptionRequest);
      isSubscribed = true; // TODO: Check response success
    }

    uint64_t now = millis();

    if((now - heartbeatTimestamp) > HEARTBEAT_INTERVAL) {
      heartbeatTimestamp = now;

      DLN("Sending heartbeat");
      webSocket.sendTXT("");
    }

  }
  
}

