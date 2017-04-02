

#include <Arduino.h>

// Configuration
const char ssid = "aalto open";
const char pass = "";
#define RelayPin 13;

void processSchedules(const char * message);
void processUsers(const char * message);
void sendReadSchedules() {
    auto res = sendOMI(scheduleRead);
    processSchedules(res);
    processUsers(res);
};


void relayOn(){ digitalWrite(RelayPin, HIGH); };
void relayOff(){ digitalWrite(RelayPin, LOW); };


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
        updateSchedules();
}


