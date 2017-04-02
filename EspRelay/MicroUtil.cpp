
#include <Arduino.h>
#include <pgmspace.h>
#include "MicroUtil.h"

void waitForHuman(uint8_t seconds) {
    for(uint8_t t = seconds; t > 0; t--) {
        D("WAIT"); DBG_NUM(t);
        DEBUG_PORT.flush();
        delay(1000);
    }
}

char _msgBuf[120];
char* _getMsgBuf(const __FlashStringHelper* fstr) {
    strncpy_P(_msgBuf, reinterpret_cast<PGM_P>(fstr), sizeof(_msgBuf));
    _msgBuf[sizeof(_msgBuf) - 1] = '\0';
    return _msgBuf;
}

