#ifndef CONFIG_H
#define CONFIG_H




///////////////////
// O-MI settings

// 1   is version 1 of the standards
// 1.1 is improved version of v1 with only non-functional schema improvements so it might be marked as v1 in the standard
// 1.2 is non-official temporary version which is combination of v1.1 and upcoming v2 features
// 2   is upcoming, mostly backwards compatible version 2 of standards
#define OMI_VERSION 1
#define OMI_TTL "30" //sec

#define DatabaseReservations 10
#define DatabaseUsers 50


#define OMI_PROTO "http://"
#define OMI_HOST "biotope.cs.hut.fi"
#define OMI_PATH "/omi/node/"
#define OMI_URL OMI_HOST OMI_PATH
#define OMI_CERT_FINGERPRINT String("6D 7F AE 98 E6 4A 74 76 45 26 7F 66 14 3C 9F 58 43 CB 09 B5")

#define START 0xAA
#define CURRENT_VALUE 0x00
#define POWER_VALUE 0x00
#define RELAY_ON 0x0F
#define RELAY_OFF 0x03
#define STOP 0xAA


///////////////////////
// PIN CONFIGURATION
	
// GSM module
#define SerialAT Serial
#define DEBUG_ESP_PORT Serial1

#define PIN_GSM_PWR  D3
#define PIN_LOCK     D1
#define PIN_SONOFF_RX D5
#define PIN_SONOFF_TX D6 // TODO: connect or not?





///////////////////
// Timing source // TODO: Is this needed, no wifi available so esp libraries are useless

//#define TIMEZONE 0
//
//#include <time.h>
//#ifdef __cplusplus
//extern "C"{
//#endif
//#include <sntp.h>
//struct tm * ICACHE_FLASH_ATTR sntp_localtime(const time_t * tim_p);
//#ifdef __cplusplus
//} // extern "C"
//#endif




/////////////////
// GPRS settings

// Select your modem:
//#define TINY_GSM_MODEM_SIM800
//#define TINY_GSM_MODEM_SIM900
//#define TINY_GSM_MODEM_M590
#define TINY_GSM_MODEM_A7

const char apn[]  = "internet"; // elisa prepaid + many others in Finland
const char user[] = "";
const char pass[] = "";

// activate custom GSM support; not supported in the original at the time of writing, 2017-05
#define WEBSOCKETS_NETWORK_TYPE NETWORK_A7_GSM
#include "TinyGSM/TinyGsmClient.h"
#include "arduinoWebSockets/src/WebSocketsClient.h"
#define HTTPCLIENT WebSocketsClient




#endif // CONFIG_H
