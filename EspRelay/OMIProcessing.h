#ifndef OMI_PROCESSING_H
#define OMI_PROCESSING_H


#include "WString.h"
#include "Config.h"


// Simple http POST wrapper
// Returns response as string
String sendOMI(HTTPCLIENT& http, const char* request);

int getReturnCode(String& response);

// Find Object (with <Object> tags) specified by objectId, if empty objectId then return the first found Object
// Return true on success. Uses linear search
bool findObject(const String& xml, const String& objectId, String& result, unsigned& processedChars);
//bool findObject(const String& xml, const String& objectId, String& result, unsigned& processedChars = 0);

// Find InfoItem specified by infoItemName, if empty name then return first found infoitem
// Return true on success. Uses linear search
bool findInfoItem(const String& xml, const String& infoitemName, String& result, unsigned& processedChars);
//bool findInfoItem(const String& xml, const String& infoitemName, String& result, unsigned& processedChars = 0);

// Get the first value, search starting fromIndex
// Result will be set to the contents of value (without open/close tags)
bool findValue(const String& xml, String& result, unsigned& fromIndex);
//bool findValue(const String& xml, String& result, unsigned fromIndex = 0);

const char tagEnd = '>';
#define NOT_FOUND -1

#endif
