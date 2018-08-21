#ifndef MQTT__H
#define MQTT__H

void publishMessage();
void onMessageReceived(String topic, String message);
void startMqttClient();
void stopMqttClient();

#endif