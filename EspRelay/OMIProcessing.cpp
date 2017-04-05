
#include <Arduino.h>
#include <progmem.h>
#include "OMIProcessing.h"

String sendOMI(HTTPCLIENT & http, const char* request) {
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

int getReturnCode(String& response) {
    String needle(F("<omi:return returnCode=\""));
    String needle2(F("<return returnCode=\""));

    int start = -1, end = -1;
    
    if ((start = response.indexOf(needle)) > 0)
        start += needle.length();
    if ((start = response.indexOf(needle2)) > 0)
        start += needle2.length();
    if (start > 0 &&
            (end = response.indexOf('"', start) > 0) {
        return parseInt(response.substring(start, end));
    }

}

bool findObject(const String& xml, const String& objectId, String& result, unsigned fromIndex) {
    String objectStart(F("<Object>"));
    String objectEnd(F("</Object>"));
    String idStart(F("<id>"));
    String idEnd  (F("</id>"));

    D("[OMI-parser] Starting search for Object: "); DSLN(objectId);

    unsigned processedChars = fromIndex;
    int start = NOT_FOUND,
        end = NOT_FOUND, objStart = NOT_FOUND;
    while ( processedChars != 0 && objStart != NOT_FOUND ) {

        // find Object
        if ((objStart = xml.indexOf(objectStart, processedChars)) >= 0)
            processedChars = objStart + objectStart.length();
        else break;

        

        // find the id of the Object
        if ((start = xml.indexOf(idStart, objStart)) > 0) {

            start += idStart.length(); // start = start of id

            // find the end of the id
            if ((end = xml.indexOf(idEnd, start)) > 0) {

                String id = xml.substring(start, end).trim();
                D("[OMI-parser] processing Object: "); DS(id);
                if (! id.equals(objectId)) {
                    DLN("... skip");
                    continue;
                }

                // FOUND THE OBJECT ID
                DLN("... FOUND");

                do { // Next: find the object closing tag
                    processedChars = end;

                    if ((end = xml.indexOf(objectEnd, processedChars)) > 0)
                        end += objectEnd.length();
                    else {
                        DLN("[OMI-parser] Error: object close tag not found. Trying to recover...");
                        result = xml.substring(objStart);
                        return true;
                    }

                    if ((start = xml.indexOf(objectStart, processedChars)) == NOT_FOUND)
                        start = 0x7FFF;

                } while (start < end); // if find starting tag followed by closing: continue

                // found closing tag followed by (nothing or starting tag)
                result = xml.substring(objStart, end+objectEnd.length());
                return true;
            }
            else {
                D("[OMI-parser] end tag for id not found for Object:");
                DS(xml.substring(objStart, min(xml.length(), objStart + 160)));
                DLN("...");
                return false;
            }
        }
        else {
            D("[OMI-parser] id not found for Object:");
            DS(xml.substring(objStart, min(xml.length(), objStart + 160)));
            DLN("...");
            return false;
        }
    }
}

bool findInfoItem(const String& xml, const String& infoitemName, String& result, unsigned fromIndex) {
    String itemStartStr(F("<InfoItem "));
    String itemNameStartStr(F("name=\""));
    String itemEndStr(F("</InfoItem>"));

    unsigned processedChars = fromIndex;
    int start = NOT_FOUND
        , end = NOT_FOUND, itemStart = NOT_FOUND;

    D("[OMI-parser] Starting search for InfoItem: "); DSLN(infoitemName);

    while ( processedChars != 0 && itemStart != NOT_FOUND) {

        // Find the start of item
        if ((itemStart = xml.indexOf(itemStartStr, processedChars)) >= 0) {
            processedChars = itemStart + itemStartStr.length();
        }
        else break;

        // find end of starting tag
        if ((end = xml.indexOf(tagEnd, itemStart)) == NOT_FOUND) {
            DLN("[OMI-parser] tag end not found");
            return false;
        }

        // find the name between itemStart and end
        if ((start = xml.indexOf(itemNameStartStr, itemStart)) != NOT_FOUND 
                && start < end) {
            start += itemNameStartStr.length();

            // find name end between start and end
            int nameEnd = NOT_FOUND;
            if ((nameEnd = xml.indexOf('"', start)) != NOT_FOUND 
                    && nameEnd < end) {
                // name found
                String name = xml.substring(start, nameEnd);

                D("[OMI-parser] processing InfoItem: "); DS(name);
                if (!name.equals(infoitemName)) {
                    DLN("... skip");
                    continue;
                }
                DLN("... FOUND");

                // Find closing tag (there might be MetaData InfoItems)
                do {
                    processedChars = end;

                    if ((end = xml.indexOf(itemEndStr, processedChars)) == NOT_FOUND) {
                        DLN("[OMI-parser] Error: InfoItem closing tag not found. Trying to recover...");
                        result = xml.substring(itemStart);
                        return true;
                    }

                    if ((start = xml.indexOf(itemStartStr, processedChars)) == NOT_FOUND) {
                        start = 0x7FFF;
                    }
                } while ( end > start );

                result = xml.substring(itemStart, end);
                return true
            }
            else {
                DLN("[OMI-parser] Error: no name end");
            }

        }
        else {
            DLN("[OMI-parser] Error: no name");
            return false;
        }
    }
}



