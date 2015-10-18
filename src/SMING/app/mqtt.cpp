#include <app.h>

/**
 * @file
 * @brief All your MQTT related stuff
 */

MqttClient xMqtt(MQTT_BROKER, MQTT_PORT, mqtt_onMessageReceive);
Timer mqttTimer;

// Publish our message


/**
 * @brief      Plublishes
 *
 * @param[in]  topic    Topic
 * @param[in]  message  Message
 */
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
 * @param[in] topic Topic on which message is received
 * @param[in] message Message string
 * 
 * @test DISCOVERY ---> mosquitto_sub -t "switch-case/find-rep" + mosquitto_pub -t "switch-case/find-req" -m "ping"
 * @test CONTROL   ---> mosquitto_sub -t "switch-case/$MAC_ADDR/ctrl-rep" + mosquitto_pub -t "switch-case/$MAC_ADDR/ctrl-req" -m "{Your Json}"
 * @test OTA       ---> mosquitto_sub -h "test.mosquitto.org" -t "switch-case/ota-rep" + mosquitto_pub -h "test.mosquitto.org" -t "switch-case/ota-req" -m "begging you to update"
 */
 void mqtt_onMessageReceive(String topic, String message)
 {
    PRINTF_INFO("@@@ Topic = %s, Message = %s @@@\n", topic.c_str(), message.c_str());

    if(topic == DISCOVERY_REQUEST_TOPIC){
        mqtt_publishMessage(DISCOVERY_REPLY_TOPIC, "discovery-reply: " + MAC_ADDR);
    }
    else if(topic == CONTROL_REQUEST_TOPIC){
        mqtt_publishMessage(CONTROL_REPLY_TOPIC, "control-reply: " + message);

    }
    else if(topic == OTA_UPDATE_TOPIC){
        PRINTF_INFO("RECIVED REQUEST ON OTA UPDATE TOPIC\n");
        system_showInfo();
        ota_update();
        //TODO: Send whether OTA is a success or failure
        mqtt_publishMessage(OTA_REPLY_TOPIC, "ota-reply: done");
    }
    else{
        PRINTF_ERR("@@@ Invalid topic @@@\n");
    }

}

/**
 * @brief Start mqtt client and subscribe to all necessary topics
 * @details Keeps trying to connect to mqtt broker in case of failure
 */
void mqtt_startClient()
{
    PRINTF_INFO("@@@ Trying MQTT Connect @@@\n");
    if(xMqtt.connect("MOSQUITTO") == true){
        mqttTimer.stop();
        
        /* Subscribe to all necessary topics */
        xMqtt.subscribe(DISCOVERY_REQUEST_TOPIC);
        xMqtt.subscribe(CONTROL_REQUEST_TOPIC);
        xMqtt.subscribe(OTA_REQUEST_TOPIC);

        PRINTF_INFO("========= MQTT Connected, Subscribed =========\n");
        PRINTF_INFO("My ID = %s\n", WifiStation.getMAC().c_str());
        PRINTF_INFO("My IP = %s\n", WifiStation.getIP().toString().c_str());
        PRINTF_INFO("==============================================\r\n");

    }
    else{
        PRINTF_ERR("Failed to connect to broker. Retrying in 10 secs....\n");
        mqttTimer.initializeMs(10 * 1000, mqtt_startClient).start(); // every 10 seconds
    }
    
}
