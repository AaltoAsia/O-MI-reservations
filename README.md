<!-- vim: tw=80 -->

# Embedded reservation system

ESP8266 project which has a reservation system. Uses [O-DF]()/[O-MI]() protocols for control.


Components
----------

* [Sonoff POW smart socket](https://www.itead.cc/smart-socket-eu.html) (€11.80)
* ESP8266 for better extension possibilities. I used
  [Wemos mini](https://www.aliexpress.com/store/product/D1-mini-Mini-NodeMcu-4M-bytes-Lua-WIFI-Internet-of-Things-development-board-based-ESP8266/1331105_32529101036.html?spm=2114.12010615.0.0.aHT9H7)
  (€3.74)
* USB to TTL adapter (3.3V recommended) for programming Sonoff (~3€)

Sum ~18.54 €


Other software needed
---------------------

Arduino with [esp8266 core](https://github.com/esp8266/Arduino) for building and
uploading programs to hardware.

[O-MI node server]() with agent for handling the reservations. This link has it
preinstalled.

* Run by starting `bin/o-mi-node.sh`
* The device needs a network connection to this server
  * Static IP or DNS address is required
  * Ports need to be open to internet if the server is not used in the same network

* I have test server running at http://otaniemi3d.cs.hut.fi/omi/node/ but it
  might require newer version from branch `master` of this repository.



Reprogramming Sonoff
-------------------

This project uses sonoff only for the relay electronics and packaging. It has
also esp8266 chip which could be used for this project, but it doesn't have
other than serial pins available easily. So for future expansion possibilities I
chose to use another esp8266 for the real program. Anyways sonoff need to be
reprogrammed, which is quite easy.

1. Open `Relay/BlinkX/BlinkX.ino` in Arduino IDE
2. Change board to `Generic ESP8266 Module`
3. Solder a pin header to Sonoff holes marked as VCC, TX, RX, GND
4. Connect USB to TTL adapter to the header (TX to RX, RX to TX, VCC to VCC,
   GND to GND)
5. Before connecting USB to TTL adapter to USB, press down the button in Sonoff
   and keep it pressed while connecting the USB cable.
6. Select right port from Arduino IDE
7. Press Upload

This program only listens for "I" or "O" from serial and sets the relay to
corresponding state.


Programming ESP8266 Module
-------------------------

1. Connect USB cable to ESP8266 and computer.
2. Open Arduino IDE to open `EspRelay/EspRelay.ino` or use Makefile with `make upload` 
3. Change Settings for your Module (WeMos Mini for me)
4. Upload

Assembling
---------

1. Connect Sonoff serial to esp8266 module serial (TX to RX, RX to TX)
2. Connect VCC to VCC and GND to GND
3. Done.



