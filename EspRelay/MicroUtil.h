
#include <stdint.h>
#include <pgmspace.h>
#include <WString.h>


// Enables the use of ';' after it but does nothing
#define NOP do {} while (false)

// DEBUGGING Helpers
#ifdef DEBUG_ESP_PORT
#define DEBUG_PORT DEBUG_ESP_PORT
#define DFORMAT(...) DEBUG_ESP_PORT.printf( __VA_ARGS__ )
#else
#define DFORMAT(...) NOP
#endif

#ifdef DEBUG_PORT
#define D(m) DEBUG_PORT.print(F(m))
#define DLN(m) DEBUG_PORT.println(F(m))
#define DS(m) DEBUG_PORT.print(m)
#define DSLN(m) DEBUG_PORT.println(m)
#define DNUM(n) DEBUG_PORT.print(String(n))
#define DNUMLN(n) DEBUG_PORT.println(String(n))
#define DFLUSH DEBUG_PORT.flush()
#else
#define D(m) NOP
#define DLN(m) NOP
#define DS(m) NOP
#define DSLN(m) NOP
#define DNUM(n) NOP
#define DNUMLN(n) NOP
#define DFLUSH NOP
#endif


// Wait while printing the remaining wait time with DEBUG() and DEBUG_PORT
void waitForHuman(uint8_t seconds);

// Helper for storing constant strings to flash and reading them as normal c-strings
// Example: stream.printf(FS("Long formatting string for function that doesn't support String!"))
char* _getMsgBuf(const __FlashStringHelper* fstr);
#define FS(str) _getMsgBuf(F(str))
#define FString(str) String(F(str))

#ifndef MSGBUF_SIZE
#define MSGBUF_SIZE 120
#endif

#ifndef MSGBUFL_SIZE
#define MSGBUFL_SIZE 400
#endif

extern char _msgBuf[MSGBUF_SIZE];
extern char _msgBufL[MSGBUFL_SIZE];
//extern char* _msgBuf;
//extern char* _msgBufL;

char* loadFashString(const __FlashStringHelper* fstr, char * buffer, size_t len);

