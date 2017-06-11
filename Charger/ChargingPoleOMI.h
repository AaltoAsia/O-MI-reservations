#ifndef CHARGING_POLE_OMI
#define CHARGING_POLE_OMI

#include <pgmspace.h>
#include "MicroUtil.h"
#include "Config.h"
#include "types.h"

#if OMI_VERSION == 1
#   define OMI_NS "omi.xsd"
#   define ODF_NS "odf.xsd"
#elif OMI_VERSION < 2
#   define OMI_NS "http://www.opengroup.org/xsd/omi/1.0/"
#   define ODF_NS "http://www.opengroup.org/xsd/odf/1.0/"
#elif OMI_VERSION == 2
#   define OMI_NS "http://www.opengroup.org/xsd/omi/2.0/"
#   define ODF_NS "http://www.opengroup.org/xsd/odf/2.0/"
#else
#   error("Given OMI_VERSION not supported")
#endif

#define ReservationsObjectId FString("Reservations")
#define UserIDId        FString("UserID")
#define StartsAfterName FString("StartsAfter")
#define EndsAfterName   FString("EndsAfter")


#define UsersObjectId   FString("Users")
#define AuthId          FString("Auth")
#define NFCName         FString("NFC")
#define MACName         FString("MAC")
#define PassName        FString("PassMD5")

// TODO: split timeframe into two timeframes if too large response with begin&end
      //"<read msgformat=\"odf\" begin=\"%s\">" // e.g. 2017-04-04T20:20:00.000+02:00
const char readBeginTemplate[] PROGMEM =
    "<omiEnvelope xmlns=\"" OMI_NS "\" version=\"1.0\" ttl=\"" OMI_TTL "\">"
      "<read msgformat=\"odf\">" 
        "<msg>"
          "<Objects xmlns=\"" ODF_NS "\">"
            "<Object>"
              "<id>ChargingPole</id>"
              "<Object>"
                "<id>Users</id>"
              "</Object>"
              "<Object>"
                "<id>Reservations</id>"
              "</Object>"
            "</Object>"
          "</Objects>"
        "</msg>"
      "</read>"
    "</omiEnvelope>";

const char callRequest[] PROGMEM =
    "<omiEnvelope xmlns=\"" OMI_NS "\" version=\"1.0\" ttl=\"" OMI_TTL "\">"
      "<call msgformat=\"odf\">"
        "<msg>"
          "<Objects xmlns=\"" ODF_NS "\">"
            "<Object>"
              "<id>ChargingPole</id>"
              "<InfoItem name=\"GetData\" />"
            "</Object>"
          "</Objects>"
        "</msg>"
      "</read>"
    "</omiEnvelope>";


char * readUpdate(const char* lastUpdateTimestamp);
// Declarations
void processReservations(String& message, DB& database);
void processUsers(String& message, DB& database);


#endif
