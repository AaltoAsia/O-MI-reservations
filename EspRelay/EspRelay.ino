

#include <Arduino.h>
#include "MicroUtil.h"
//#include "OMI.h"
#include "OMIProcessing.h"

// Configuration
const char ssid = "aalto open";
const char pass = "";
#define RelayPin 13;


// Declarations
void processReservations(const char * message);
void processUsers(const char * message);

void relayOn(){ digitalWrite(RelayPin, HIGH); };
void relayOff(){ digitalWrite(RelayPin, LOW); };

void synchronizeData() {
    auto res = sendOMI(readUpdate(lastUpdate));
    String code = getReturnCode(res);

    if (code == 200) {
        processReservations(res);
        processUsers(res);
    }
};





void setup() {
    // serialSetup();
    Serial.begin(115200);

#   ifdef DEBUG_ESP_PORT
        DEBUG_ESP_PORT.setDebugOutput(true);
        waitForHuman();
#   endif

    pinMode(RelayPin, OUTPUT);
    relayOff();
}

void loop() {
    // every interval
    if ((millis() / 1000) % (15 * 60) == 0)
        synchronizeData();
}


