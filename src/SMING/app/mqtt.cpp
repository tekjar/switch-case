#include <app.h>

MqttClient xMqtt(MQTT_BROKER, MQTT_PORT, mqtt_onMessageReceive);
Timer mqttTimer;

// Publish our message
void mqtt_publishMessage(String topic, String message)
{
    if (xMqtt.getConnectionState() != eTCS_Connected)
        mqtt_startClient(); // Auto reconnect

    Serial.println("@@@ Let's publish message now! @@@");
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
/**
 * @brief Callback when there is a message on any of the subscription
 * @details All the processing of messages will be done here
 * 
 * @param topic topic on which message is received
 * @param message message string
 * 
 * @test mosquitto_sub -t "switch-case/find-rep" + mosquitto_pub -t "switch-case/find-req" -m "ping"
 * @test mosquitto_sub -t "switch-case/$MAC_ADDR/ctrl-rep" + mosquitto_pub -t "switch-case/$MAC_ADDR/ctrl-req" -m "{Your Json}"
 */
 void mqtt_onMessageReceive(String topic, String message)
 {
    Serial.printf("@@@ Topic = %s, Message = %s @@@\n", topic.c_str(), message.c_str());

    if(topic == DISCOVERY_REQUEST_TOPIC){
        mqtt_publishMessage(DISCOVERY_REPLY_TOPIC, "discovery-reply: " + MAC_ADDR);
    }
    else if(topic == CONTROL_REQUEST_TOPIC){
        mqtt_publishMessage(CONTROL_REPLY_TOPIC, "control-reply: " + message);

    }
    else{
        Serial.printf("@@@ Invalid topic @@@\n");
    }

}

/**
 * @brief Start mqtt client and subscribe to all necessary topics
 * @details Keeps trying to connect to mqtt broker
 */
void mqtt_startClient()
{
    Serial.println("@@@ Trying MQTT Connect @@@");
    if(xMqtt.connect("MOSQUITTO") == true){
        mqttTimer.stop();
        
        Serial.println("@@@ MQTT CONNECTED @@@");

        /* Subscribe to all necessary topics */
        xMqtt.subscribe(DISCOVERY_REQUEST_TOPIC);
        xMqtt.subscribe(CONTROL_REQUEST_TOPIC);

        Serial.println("\r\n=== MQTT CLIENT STARTED ===");
        Serial.println(WifiStation.getIP());
        Serial.println("==============================\r\n");

    }
    else{
        mqttTimer.initializeMs(10 * 1000, mqtt_startClient).start(); // every 20 seconds
    }
    
}
