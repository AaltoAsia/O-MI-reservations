#ifndef CHARGING_POLE_OMI
#define CHARGING_POLE_OMI

#include <progmem.h>

const char readBeginTemplate[] PROGMEM =
    "<omiEnvelope xmlns=\"omi.xsd\" version=\"1.0\" ttl=\"0\">"
      "<read msgformat=\"odf\" begin=\"%s\">" // e.g. 2017-04-04T20:20:00.000Z
        "<msg>"
          "<Objects xmlns=\"odf.xsd\"/>"
        "</msg>"
      "</read>"
    "</omiEnvelope>";

char * readUpdate(char* lastUpdateTimestamp){
    return sprintf_P(_msgBufL, readBeginTemplate, lastUpdateTimestamp);
};


#endif
