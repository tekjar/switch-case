#include <SmingCore/SmingCore.h>
#include <app.h>

//#define MQTT_BROKER "192.168.0.134"
#define MQTT_BROKER "test.mosquitto.org"
#define MQTT_PORT 1883

MqttClient xMqtt(MQTT_BROKER, MQTT_PORT, mqtt_onMessageReceive);

// Publish our message
void mqtt_publishMessage()
{
    if (xMqtt.getConnectionState() != eTCS_Connected)
        mqtt_startClient(); // Auto reconnect

    Serial.println("Let's publish message now!");
    xMqtt.publish("main/frameworks/sming", "Hello friends, from Internet of things :)"); // or publishWithQoS
}

/* NOTES: Parses the json and control appropriate devices
 *
 * EXAMPLES:
 * --------
 * mosquitto_pub -t "main/status/hello" -m "{\"switchboard\":{\"ON\": $((2#00000001))}}"                           --> Switch ON device 1
 * mosquitto_pub -t "main/status/hello" -m "{\"switchboard\":{\"ON\": $((2#00000111))}}"                           --> Switch ON device 1, 2, 3
 * mosquitto_pub -t "main/status/hello" -m "{\"switchboard\":{\"OFF\": $((2#00000111))}}"                          --> Switch OFF device 1, 2, 3
 * mosquitto_pub -t "main/status/hello" -m "{\"switchboard\":{\"ON\": $((2#00000001)), \"OFF\": $((2#00000110))}}" --> Switch ON device 1 and Switch OFF device 2 and 3
 *
 */
void mqtt_onMessageReceive(String topic, String message)
{
    Serial.print(topic);
    Serial.print(":\r\n\t"); // Prettify alignment for printing
    Serial.println(message);

    json_parseAndExecute(message);
}

// Run MQTT client
void mqtt_startClient()
{
    Serial.println("Trying MQTT Connect");
    xMqtt.connect("MOSQUITTO");
    Serial.println("MQTT CONNECTED");
    xMqtt.subscribe("main/status/#");

    Serial.println("\r\n=== MQTT CLIENT STARTED ===");
	Serial.println(WifiStation.getIP());
	Serial.println("==============================\r\n");
}
