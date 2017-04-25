<!-- vim: tw=80 -->

# Embedded reservation system

ESP8266 project which has a reservation system. Uses [O-DF]()/[O-MI]() protocols for control.


Components
----------

* ``


Other software needed
---------------------

[O-MI node server]() with agent for handling the reservations. This link has it
preinstalled.

* Run by starting `bin/o-mi-node.sh`
* The device needs a network connection to this server
  * Static IP or DNS address is required
  * Ports need to be open to internet if the server is not used in the same network

* I have test server running at http://otaniemi3d.cs.hut.fi/omi/node/ but it
  might require newer version from branch `master` of this repository.


