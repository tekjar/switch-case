#include <user_config.h>
#include <SmingCore/SmingCore.h>

// If you want, you can define WiFi settings globally in Eclipse Environment Variables
#ifndef WIFI_SSID
#define WIFI_SSID "OpenWrt" // Put you SSID and Password here
#define WIFI_PWD "abc@12345"
#endif

// Forward declarations
void startMqttClient();
void onMessageReceived(String topic, String message);

Timer procTimer;

// MQTT client
// For quickly check you can use: http://www.hivemq.com/demos/websocket-client/ (Connection= test.mosquitto.org:8080)
MqttClient mqtt("192.168.0.134", 1883, onMessageReceived);

// Publish our message
void publishMessage()
{
    if (mqtt.getConnectionState() != eTCS_Connected)
        startMqttClient(); // Auto reconnect

    Serial.println("Let's publish message now!");
    mqtt.publish("main/frameworks/sming", "Hello friends, from Internet of things :)"); // or publishWithQoS
}

// Callback for messages, arrived from MQTT server
void onMessageReceived(String topic, String message)
{
    Serial.print(topic);
    Serial.print(":\r\n\t"); // Prettify alignment for printing
    Serial.println(message);

    char switch_message[100] = {0};
    sprintf(switch_message, "%s", message.c_str());

    StaticJsonBuffer<200> json_buff;
    JsonObject& root = json_buff.parseObject(switch_message);

    if(!root.success()){
        Serial.println("Parsing failed");
        return;
    }
#if 0
    JsonObject& switchboard = root["switchboard"];

    if(switchboard.containsKey("ON")){
        String status = switchboard["ON"].toString();
        Serial.println("@@@@@@ ON" +  status);
    }

    if(switchboard.containsKey("OFF")){
        String status = switchboard["OFF"].toString();
        Serial.println("@@@@@@ OFF" + status);
    }

    root.printTo(Serial);
    Serial.println("");
#endif

    /*NOTES:
     *
     *mosquitto_pub -t "main/status/hello" -m "{\"switchboard\":{\"ON\": 100, \"OFF\": 200, \"TEST\":400}}"
     */

    //add validation for this when containsKey() method is working
    JsonObject& switchboard = root["switchboard"];

    for (JsonObject::iterator it = switchboard.begin(); it != switchboard.end(); ++it)
    {
        String key = it->key;
        
        if(key == "ON"){
            Serial.println("@@@@@ ON");
        }
        else if(key == "OFF"){
            Serial.println("@@@@@ OFF");
        }
        else
            Serial.println("@@@@@ INVALID KEY");
    }
}

// Run MQTT client
void startMqttClient()
{
    Serial.println("Trying MQTT Connect");
    mqtt.connect("MOSQUITTO");
    Serial.println("MQTT CONNECTED");
    mqtt.subscribe("main/status/#");
}

// Will be called when WiFi station was connected to AP
void connectOk()
{
    Serial.println("I'm CONNECTED");

    // Run MQTT client
    startMqttClient();

    // Start publishing loop
    procTimer.initializeMs(20 * 1000, publishMessage).start(); // every 20 seconds
}

// Will be called when WiFi station timeout was reached
void connectFail()
{
    Serial.println("I'm NOT CONNECTED. Need help :(");

    // .. some you code for device configuration ..
}

void init()
{
    Serial.begin(SERIAL_BAUD_RATE); // 115200 by default
    Serial.systemDebugOutput(true); // Debug output to serial

    WifiStation.config(WIFI_SSID, WIFI_PWD);
    WifiStation.enable(true);
    WifiAccessPoint.enable(false);

    // Run our method when station was connected to AP (or not connected)
    WifiStation.waitConnection(connectOk, 20, connectFail); // We recommend 20+ seconds for connection timeout at start
}
