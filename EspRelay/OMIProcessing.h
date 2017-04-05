#ifndef OMI_PROCESSING_H
#define OMI_PROCESSING_H


#include "WString.h"

// Simple http POST wrapper
// Returns response as string
String sendOMI(HTTPCLIENT & http, char* request);

int getReturnCode(String& response);

// Return Object (with <Object> tags) specified by objectId 
// Uses linear search
bool findObject(const String& xml, const String& objectId, String& result, unsigned fromIndex);

bool findInfoItem(const String& xml, const String& infoitemName, String& result, unsigned fromIndex);

const char tagEnd = '>';
#define NOT_FOUND -1

#endif
