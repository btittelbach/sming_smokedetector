#include <SmingCore/SmingCore.h>
#include <spiffsconfig.h>
#include "pinconfig.h"
#include "mqtt.h"

//////////////////////////////////
/////// MQTT Stuff ///////////////
//////////////////////////////////


Timer procMQTTTimer;
MqttClient *mqtt = nullptr;

String getMQTTTopic(String topic3)
{
	return NetConfig.mqtt_lvl1_topic+"/"+NetConfig.mqtt_clientid+"/"+topic3;
}


// Check for MQTT Disconnection
void checkMQTTDisconnect(TcpClient& client, bool flag){

	debugf("MQTT Broker connection failed");
	// Called whenever MQTT connection is failed.
	if (flag == true)
	{
		debugf("MQTT Broker Disconnected!!");
	}
	// Restart connection attempt after few seconds
	// changes procMQTTTimer callback function
	procMQTTTimer.initializeMs(5 * 1000, startMqttClient).start(); // every 5 seconds
}

void onMessageDelivered(uint16_t msgId, int type) {
	//Serial.printf("Message with id %d and QoS %d was delivered successfully.", msgId, (type==MQTT_MSG_PUBREC? 2: 1));
}

void publishSmokeAlarm()
{
	if (nullptr == mqtt)
		return;

	StaticJsonBuffer<1024> jsonBuffer;
	String message;

	JsonObject& root = jsonBuffer.createObject();
	root[JSONKEY_ROOM] = NetConfig.location;
	root[JSONKEY_BATTVOLTAGE] = 9*analogRead(BATTERY_ADC_PIN)/1024; //TODO: Spannungsteiler besser einrechnen, 0..9V auf 0..3.3V mappen
	root.printTo(message);
	//publish to myself (where presumably everybody else also listens), the current settings
	mqtt->publish(getMQTTTopic(MQTT_TOPIC3_ALARM), message, false);
}

// Publish our message
void publishMessage()
{
	debugf("MQTT publish message");
	publishSmokeAlarm();
}


// Callback for messages, arrived from MQTT server
void onMessageReceived(String topic, String message)
{
	// debugf("topic: %s",topic.c_str());
	// debugf("msg: %s",message.c_str());
	//GRML BUG :-( It would be really nice to filter out retained messages,
	//             to avoid the light powering up, going into defaultlight settings, then getting wifi and switching to a retained /light setting
	//GRML :-( unfortunately we can't distinguish between retained and fresh messages here
	debugf("MQTT msg recv: %s | %s", topic.c_str(), message.c_str());

}

void setLastWillOrAnnounceOnlineState(bool online)
{
	//prepare last will // online msg
	debugf("MQTT prepare last will");
	StaticJsonBuffer<256> jsonBuffer;
	String message;
	JsonObject& root = jsonBuffer.createObject();
	root[JSONKEY_IP] = WifiStation.getIP().toString();
	root[JSONKEY_ONLINE] = online;
	root.printTo(message);
	if (online)
	{
		debugf("MQTT publish online msg");
		mqtt->publish(getMQTTTopic(MQTT_TOPIC3_DEVICEONLINE),message,true);
	} else {
		debugf("MQTT set last will");
		mqtt->setWill(getMQTTTopic(MQTT_TOPIC3_DEVICEONLINE),message,0,true);
	}
}

// Run MQTT client, connect to server, subscribe topics
void startMqttClient()
{
	procMQTTTimer.stop();

	if (nullptr == mqtt)
	{
		debugf("MQTT instantinating new client");
		mqtt = new MqttClient(NetConfig.mqtt_broker, NetConfig.mqtt_port, onMessageReceived);
	}

/*	if(!mqtt->setWill("last/will","The connection from this device is lost:(", 1, true)) {
		debugf("Unable to set the last will and testament. Most probably there is not enough memory on the device.");
	}
*/
	mqtt->setKeepAlive(42);
	mqtt->setPingRepeatTime(21);
	bool usessl=false;
#ifdef ENABLE_SSL
	usessl=true;
	mqtt->addSslOptions(SSL_SERVER_VERIFY_LATER);

	mqtt->setSslClientKeyCert(default_private_key, default_private_key_len,
							  default_certificate, default_certificate_len, NULL, true);
#endif

	//set last will, must be done before connecting
	setLastWillOrAnnounceOnlineState(false);

	debugf("MQTT connecting to broker");
	mqtt->connect(NetConfig.mqtt_clientid, NetConfig.mqtt_user, NetConfig.mqtt_pass, true);
	debugf("MQTT connected to broker");

	// Assign a disconnect callback function
	mqtt->setCompleteDelegate(checkMQTTDisconnect);

	// send online msg
	setLastWillOrAnnounceOnlineState(true);

	//enable periodic status updates
	procMQTTTimer.initializeMs(15 * 1000, publishMessage).start(); // every 15 seconds

	publishMessage();
}

void stopMqttClient()
{
	mqtt->setKeepAlive(0);
	mqtt->setPingRepeatTime(0);
	procMQTTTimer.stop();
}

