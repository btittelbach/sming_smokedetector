#ifndef USERCONFIG_H
#define USERCONFIG_H

#define BATTERY_ADC_PIN A0 //D3 10k pullup on D1mini
//#define PIR_PIN 2 //D4 10k pullup on D1mini, BUILTIN LED
//#define PIR_PIN 4 //D2 10k pulldown on D1mini
//#define PIR_PIN 12 //D6

const String JSONKEY_IP = "ip";
const String JSONKEY_ONLINE = "online";
const String JSONKEY_ROOM = "room";
const String JSONKEY_BATTVOLTAGE = "voltage";

const String MQTT_TOPIC3_ALARM = "smokealarm";
const String MQTT_TOPIC3_DEVICEONLINE = "online";


#endif // USERCONFIG_H
