
#include "OMIProcessing.h"

String sendOMI(HTTPCLIENT & http, char* request) {
    yield();

    DLN("[HTTP] begin...");
    http.begin(OMI_URL, OMI_CERT_FINGERPRINT);

    http.addHeader(FS("Content-Type"), "text/xml");
    DLN("[HTTP] POST... with payload: ");
    DSLN(request);
    
    int httpCode = 
        http.POST((uint8_t*)omiHttpBuffer, strlen(omiHttpBuffer));


    // httpCode will be negative on error
    if(httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        D("[HTTP] POST done, code: "); DNUMLN(httpCode);


        // print result into serial
        String payload = http.getString();
        http.end();
        DSLN(payload);
        DLN("=================== EOF ====================");

        // Success
        //if(httpCode == HTTP_CODE_OK)

        return payload;

    } else {
        http.end();
        D("[HTTP] POST... failed, error: ");
        DSLN(http.errorToString(httpCode).c_str());
        return String("");
    }
}
