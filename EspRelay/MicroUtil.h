
#include <stdint.h>

// DEBUGGING Helpers
#ifdef DEBUG_ESP_PORT
#define DEBUG_PORT DEBUG_ESP_PORT
#define DEBUG_MSG(...) DEBUG_ESP_PORT.printf( __VA_ARGS__ )
#else
#define DEBUG_MSG(...)
#endif

#ifdef DEBUG_PORT
#define DLN(m) DEBUG_PORT.println(F(m))
#define D(m) DEBUG_PORT.print(F(m))
#define DBGNUM(n) DEBUG_PORT.print(String(n))
#else
#define D(m)
#define DLN(m)
#endif


// Wait while printing the remaining wait time with DEBUG() and DEBUG_PORT
void waitForHuman(uint8_t seconds);

// Helper for storing constant strings to flash and reading them as normal c-strings
// Example: stream.printf(FS("Long formatting string for function that doesn't support String!"))
char* _getMsgBuf(const __FlashStringHelper* fstr);
#define FS(str) _getMsgBuf(F(str))


