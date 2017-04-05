#ifndef CHARGING_POLE_OMI
#define CHARGING_POLE_OMI

#include <progmem.h>


#define ReservationObjectId "Reservations"
#define UsersObjectId "Users"

// TODO: split timeframe into two timeframes if too large response with begin&end
const char readBeginTemplate[] PROGMEM =
    "<omiEnvelope xmlns=\"omi.xsd\" version=\"1.0\" ttl=\"0\">"
      "<read msgformat=\"odf\" begin=\"%s\">" // e.g. 2017-04-04T20:20:00.000+02:00
        "<msg>"
          "<Objects xmlns="odf.xsd">"
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

char * readUpdate(const char* lastUpdateTimestamp){
    return sprintf_P(_msgBufL, readBeginTemplate, lastUpdateTimestamp);
};



#endif
