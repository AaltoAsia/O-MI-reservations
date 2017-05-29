

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <SoftwareSerial.h>
#include "MicroUtil.h"
//#include "OMI.h"
#include "Config.h"
#include "ChargingPoleOMI.h"
#include "OMIProcessing.h"
#include "types.h"

// GLOBALS
HTTPCLIENT http;
DB database;
ESP8266WiFiMulti WiFiMulti;
#ifndef RELAY_SERIAL
SoftwareSerial swSer(14, 12); // RX, TX
#define RELAY_SERIAL swSer
#endif

//void relayOn(){ digitalWrite(RelayPin, HIGH); };
//void relayOff(){ digitalWrite(RelayPin, LOW); };
void relayOn(){ RELAY_SERIAL.print("I"); RELAY_SERIAL.flush(); DLN("[RELAY] ON"); };
void relayOff(){ RELAY_SERIAL.print("O"); RELAY_SERIAL.flush(); DLN("[RELAY] OFF"); };

time_t getCurrentTime() { return time((time_t*)0); };

void initTime() {
    sntp_init();
    sntp_setservername(0, (char*)"fi.pool.ntp.org");
    sntp_setservername(1, (char*)"time.windows.com");
    sntp_setservername(2, (char*)"time.nist.gov");
    sntp_set_timezone(TIMEZONE);

};

bool synchronizeData() {
    DLN("[Sync]");
    //auto res = sendOMI(http, readUpdate(lastUpdate));
    auto res = sendOMI(http, readUpdate(""));
    int code = getReturnCode(res);

    if (code == 200) {
        processReservations(res, database);
        processUsers(res, database);
        return true;
    } else {
        D("[OMI-parser] Return code: "); DNUMLN(code);
        return false;
    }
};

//void LL2Serial(long long ll)
//{
//    uint64_t xx = ll/1000000000ULL;
//    if (xx >0) Serial.print((long)xx);
//    Serial.print((long)(ll-xx*1000000000));
//}

void checkStatus() {
    time_t time = getCurrentTime();
    D("[Status] Date: "); DNUMLN(time);
    bool on = false;
    for AllReservations {
        Reservation& r = database.reservations[i];
        if (r.active) {
            D("[TimeCheck] active reservation (start, end): ");
            auto start = r.unixStartTime;
            auto end = r.unixEndTime;
            DNUM(start); D(", "); DNUM(end);
            if (time > start && time < end) {
                on = true;
                break;
            }
        }
    }
    if (on) relayOn();
    else relayOff();
};



void setup() {
    // serialSetup();
    Serial.begin(115200);
    RELAY_SERIAL.begin(115200);

#   ifdef DEBUG_ESP_PORT
        DEBUG_ESP_PORT.setDebugOutput(true);
        waitForHuman(5);
#   endif

    relayOn();

    WiFiMulti.addAP(FS("aalto open"));

    http.setReuse(true);

    uint8_t wifiCounter;
    for (wifiCounter = 0; wifiCounter <= 60; wifiCounter++) {
        if (WiFiMulti.run() == WL_CONNECTED) {
            if (!synchronizeData()){
                DLN("[SETUP] Error: O-MI node link failed!");
                delay(1000);
                ESP.restart();
            } else break;
        }
        delay(4000);
    }
    if (wifiCounter >= 60) {
            DLN("[SETUP] O-MI node link sending failed");
            delay(1000);
            ESP.restart();
    }

    initTime();
    relayOff();
}

void loop() {
    // every interval
    checkStatus();
    delay(2000);
    checkStatus();
    delay(2000);
    checkStatus();
    delay(2000);
    checkStatus();
    delay(2000);
    checkStatus();
    delay(2000);
    checkStatus();
    delay(2000);
    synchronizeData();
}


