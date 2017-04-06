#ifndef CONFIG_H
#define CONFIG_H

// 1   is version 1 of the standards
// 1.1 is improved version of v1 with only non-functional schema improvements so it might be marked as v1 in the standard
// 1.2 is non-official temporary version which is combination of v1.1 and upcoming v2 features
// 2   is upcoming, mostly backwards compatible version 2 of standards
#define OMI_VERSION 1
#define OMI_TTL "30" //sec

#define DatabaseReservations 10
#define DatabaseUsers 50

#include <ESP8266HTTPClient.h>
#define HTTPCLIENT HTTPClient

#define OMI_URL "https://otaniemi3d.cs.hut.fi/omi/node/"
#define OMI_CERT_FINGERPRINT String("6D 7F AE 98 E6 4A 74 76 45 26 7F 66 14 3C 9F 58 43 CB 09 B5")

#endif
