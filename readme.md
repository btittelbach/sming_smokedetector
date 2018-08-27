Smoke Detector Alert via MQTT / Wifi
====================================

It's firmware for an ESP8266 written in C++ using the Sming Framework. It is meant to be powered up by a smoke-detector once smoke is detected (thus not drawing any power from the smoke-detector battery otherwise). It will then connect to WIFI and an MQTT broker and send an MQTT message containing the room name and the current battery voltage.

The battery voltage assumes a voltage divider of 180K Ohms to 100K Ohms supplied by the 9V battery and connected to Wemos A0.

Configuration
-------------

Copy and modify the <tt>makeconfig_example.py</tt> and run it. This will write the configuration files which will then be written to flash.
If you run a different <tt>makeconfig-*</tt> file later to flash another device, make sure to run <tt>make spiff_update</tt> before running <tt>make flash</tt>



Building
--------

* The firmware is built using Sming 3.5.2. Get it from github.com/SmingHub/Sming and follow the instruction there.
* set environment variables <tt>ESP_HOME</tt> and <tt>SMING_HOME</tt>
* <tt>./makeconfig_myroom.py</tt>
* <tt>make</tt>
* <tt>make flash</tt>


MQTT Messages sent
------------------

The message format is JSON

### LastWill and online msg

* **Topic**: e.g.: <tt>sensors/smokedetector1/online</tt>
* **Payload**: e.g.: <tt>{"ip":"192.168.1.11","online":true}</tt>


### Alert msg

* **Topic**: e.g.: <tt>sensors/smokedetector1/smokealarm</tt>
* **Payload**: e.g.: <tt>{"room":"livingroom","voltage":9.35}</tt>


