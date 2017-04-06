
#include <Arduino.h>
#include <pgmspace.h>
#include "MicroUtil.h"

void waitForHuman(uint8_t seconds) {
    for(uint8_t t = seconds; t > 0; t--) {
        D("WAIT"); DNUMLN(t);
        DFLUSH;
        delay(1000);
    }
}

char* _getMsgBuf(const __FlashStringHelper* fstr) {
    strncpy_P(_msgBuf, reinterpret_cast<PGM_P>(fstr), sizeof(_msgBuf));
    _msgBuf[sizeof(_msgBuf) - 1] = '\0';
    return _msgBuf;
}

char _msgBuf[MSGBUF_SIZE];
char _msgBufL[MSGBUFL_SIZE];

char* loadFlashString(const __FlashStringHelper* fstr, char * buffer, size_t len) {
    strncpy_P(buffer, reinterpret_cast<PGM_P>(fstr), len);
    buffer[len - 1] = '\0';
    return buffer;
}
