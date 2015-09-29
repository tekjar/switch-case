#include <app.h>

//#define MQTT_BROKER "192.168.0.134"
#define MQTT_BROKER "test.mosquitto.org"
#define MQTT_PORT 1883


MqttClient xMqtt(MQTT_BROKER, MQTT_PORT, mqtt_onMessageReceive);

// Publish our message
void mqtt_publishMessage(String topic, String message)
{
    if (xMqtt.getConnectionState() != eTCS_Connected)
        mqtt_startClient(); // Auto reconnect

    Serial.println("Let's publish message now!");
    xMqtt.publish(topic, message); // or publishWithQoS
}

/* NOTES: Parses the json and control appropriate devices
 *
 * EXAMPLES:
 * --------
 * mosquitto_pub -t "main/status/hello" -m "{\"ctrl-req\":{\"ON\": $((2#00000001))}}"                           --> Switch ON device 1
 * mosquitto_pub -t "main/status/hello" -m "{\"ctrl-req\":{\"ON\": $((2#00000111))}}"                           --> Switch ON device 1, 2, 3
 * mosquitto_pub -t "main/status/hello" -m "{\"ctrl-req\":{\"OFF\": $((2#00000111))}}"                          --> Switch OFF device 1, 2, 3
 * mosquitto_pub -t "main/status/hello" -m "{\"ctrl-req\":{\"ON\": $((2#00000001)), \"OFF\": $((2#00000110))}}" --> Switch ON device 1 and Switch OFF device 2 and 3
 *
 */
void mqtt_onMessageReceive(String topic, String message)
{
    Serial.printf("Topic = %s, Message = %s\n", topic.c_str(), message.c_str());

    if(topic == DISCOVERY_REQUEST_TOPIC){

    }
    else if(topic == CONTROL_REQUEST_TOPIC){

    }
    else{
        Serial.printf("Invalid topic\n");
    }

}

// Run MQTT client
void mqtt_startClient()
{
    Serial.println("Trying MQTT Connect");
    xMqtt.connect("MOSQUITTO");
    Serial.println("MQTT CONNECTED");

    /* Subscribe to all necessary topics */
    xMqtt.subscribe(DISCOVERY_REQUEST_TOPIC);
    xMqtt.subscribe(CONTROL_REQUEST_TOPIC);

    Serial.println("\r\n=== MQTT CLIENT STARTED ===");
	Serial.println(WifiStation.getIP());
	Serial.println("==============================\r\n");
}
