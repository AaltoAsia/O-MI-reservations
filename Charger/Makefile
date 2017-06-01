SKETCH = WebSocketTest.ino

ESP_ROOT ?= $(HOME)/Arduino/esp8266/

#LIBS += $(ESP_LIBS)/ESP8266WiFi \
#	$(ESP_LIBS)/ESP8266HTTPClient \
#	espsoftwareserial \
#	TinyGSM \
#	arduinoWebsockets \
#	.

	#$(ESP_LIBS)/ESP8266httpUpdate \

UPLOAD_PORT = /dev/ttyUSB0
UPLOAD_SPEED  = 921600
MONITOR_SPEED = 115200
BOARD = d1_mini

#FLASH_DEF = 4M3M
FLASH_DEF = 4M1M


################################################################
############# PIN CONNECTION CONFIGURATION #####################
# SERIAL CONFIGURATION:
# # Serial  UART0: Usb,           GPIO 1  TX (TX);  GPIO 3  RX (RX)
# # -DSWAPSERIAL0 puts Serial to  GPIO 15 D8 (TX);  GPIO 13 D7 (RX)
# # Serial1 UART1: RX from flash  GPIO 2  D4 (TX)


EXTRAFLAGS += -Wall -DWEBSOCKETS_NETWORK_TYPE=NETWORK_A7_GSM
# -DDEBUG_ESP_PORT=Serial1 -DSWAPSERIAL0 # -DDEBUG_ESP_HTTP_CLIENT

C_EXTRA += $(EXTRAFLAGS)
CPP_EXTRA += $(EXTRAFLAGS)



include ./makeEspArduino/makeEspArduino.mk


addr2line: $(MAIN_EXE)
	echo "Paste stack trace here and press Ctrl+D"
	egrep -o '40[0-2][0-9a-f]{5}\b' | \
	    $(TOOLS_ROOT)/xtensa-lx106-elf/bin/xtensa-lx106-elf-addr2line \
	    -aipC -e $(basename $(MAIN_EXE)).elf


upmon: upload
	screen $(UPLOAD_PORT) $(MONITOR_SPEED)
mon:
	screen $(UPLOAD_PORT) $(MONITOR_SPEED)
