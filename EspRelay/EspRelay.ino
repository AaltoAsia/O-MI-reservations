

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include "MicroUtil.h"
//#include "OMI.h"
#include "Config.h"
#include "ChargingPoleOMI.h"
#include "OMIProcessing.h"
#include "types.h"

// GLOBALS
HTTPCLIENT http;
DB database;



//void relayOn(){ digitalWrite(RelayPin, HIGH); };
//void relayOff(){ digitalWrite(RelayPin, LOW); };
void relayOn(){ Serial1.print("I"); Serial1.flush(); DLN("[RELAY] ON"); };
void relayOff(){ Serial1.print("O"); Serial1.flush(); DLN("[RELAY] OFF"); };

void synchronizeData() {
    //auto res = sendOMI(http, readUpdate(lastUpdate));
    auto res = sendOMI(http, readUpdate(""));
    int code = getReturnCode(res);

    if (code == 200) {
        processReservations(res, database);
        processUsers(res, database);
    }
};





void setup() {
    // serialSetup();
    Serial.begin(115200);
    Serial1.begin(115200);

#   ifdef DEBUG_ESP_PORT
        DEBUG_ESP_PORT.setDebugOutput(true);
        waitForHuman();
#   endif

    WiFiMulti.addAP(FString("aalto open"), NULL);

}

void loop() {
    // every interval
    if ((millis() / 1000) % (15 * 60) == 0)
        synchronizeData();
}


