#!/usr/bin/python3

from spiffsconfig import writeConfig
writeConfig(
    ip="192.168.1.11",
    nm="255.255.255.0",
    gw="192.168.1.1",
    dns=["1.1.1.1","8.8.8.8"],
    domainname=b"localdomain",
    dhcp0=True,
    wifi0_ssid=b"MyWifi",
    wifi0_pass=b"MyWifiPassword",
    location=b"livingroom",
    mqtt_port=1883,
    mqtt_broker=b"mqtt.localdomain",
    mqtt_clientid=b"smokedetector1",
    mqtt_level1topic=b"sensors",
    mqtt_user=b"",
    mqtt_pass=b"",
    authtoken=b"none")
