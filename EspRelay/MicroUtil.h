
#include <stdint.h>

// DEBUGGING Helpers
#ifdef DEBUG_ESP_PORT
#define DEBUG_PORT DEBUG_ESP_PORT
#define DFORMAT(...) DEBUG_ESP_PORT.printf( __VA_ARGS__ )
#else
#define DFORMAT(...)
#endif

#ifdef DEBUG_PORT
#define D(m) DEBUG_PORT.print(F(m))
#define DLN(m) DEBUG_PORT.println(F(m))
#define DS(m) DEBUG_PORT.print(m)
#define DSLN(m) DEBUG_PORT.println(m)
#define DNUM(n) DEBUG_PORT.print(String(n))
#define DNUMLN(n) DEBUG_PORT.println(String(n))
#else
#define D(m)
#define DLN(m)
#define DS(m)
#define DSLN(m)
#define DNUM(n)
#define DNUMLN(n)
#endif


// Wait while printing the remaining wait time with DEBUG() and DEBUG_PORT
void waitForHuman(uint8_t seconds);

// Helper for storing constant strings to flash and reading them as normal c-strings
// Example: stream.printf(FS("Long formatting string for function that doesn't support String!"))
char* _getMsgBuf(const __FlashStringHelper* fstr);
#define FS(str) _getMsgBuf(F(str))

#ifndef MSGBUF_SIZE
#define MSGBUF_SIZE 120
#endif

#ifndef MSGBUFL_SIZE
#define MSGBUFL_SIZE 400
#endif

extern _msgBuf;
extern _msgBufL;

char* loadFashString(const __FlashStringHelper* fstr, char * buffer, size_t len);

