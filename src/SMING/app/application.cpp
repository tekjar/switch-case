#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include <switches.h>

// If you want, you can define WiFi settings globally in Eclipse Environment Variables
#ifndef WIFI_SSID
#define WIFI_SSID "OpenWrt" // Put you SSID and Password here
#define WIFI_PWD "abc@12345"
#endif

//#define MQTT_BROKER "192.168.0.134"
#define MQTT_BROKER "test.mosquitto.org"
#define MQTT_PORT 1883

#define R 12
#define G 13
#define B 14

// Forward declarations
void mqtt_startClient();
void mqtt_onMessageReceive(String topic, String message);

Timer procTimer;

// MQTT client
// For quickly check you can use: http://www.hivemq.com/demos/websocket-client/ (Connection= test.mosquitto.org:8080)
MqttClient xMqtt(MQTT_BROKER, MQTT_PORT, mqtt_onMessageReceive);
HttpServer xWSServer;

int websocket_nTotalActiveSockets = 0;

void switchcase_set(COMMAND command, int value){
    switch(command){
        case ON:
            Serial.println("##### ON");
            if(value & SWITCH_1)
                digitalWrite(R, true);
            if(value & SWITCH_2)
                digitalWrite(G, true);
            if(value & SWITCH_3)
                digitalWrite(B, true);
            break;
        case OFF:
            Serial.println("#### OFF");
            if(value & SWITCH_1)
                digitalWrite(R, false);
            if(value & SWITCH_2)
                digitalWrite(G, false);
            if(value & SWITCH_3)
                digitalWrite(B, false);
            break;
        default:
            Serial.println("#### INVALID");
    }
}

/*--------------------------- JSON PARSING START ---------------------------*/
void json_parseAndExecute(String message){
    char switch_message[100] = {0};
    sprintf(switch_message, "%s", message.c_str());

    Serial.printf("Json message -> %s\n", message.c_str());

    StaticJsonBuffer<200> json_buff;
    JsonObject& root = json_buff.parseObject(switch_message);

    if(!root.success()){
        Serial.println("Parsing failed");
        return;
    }

    //add validation for this when containsKey() method is working
    JsonObject& switchboard = root["switchboard"];

    for(JsonObject::iterator it = switchboard.begin(); it != switchboard.end(); ++it)
    {
        String key = it->key;

        if(key == "ON"){
            Serial.println("@@@@@ ON");
            switchcase_set(ON, it->value);
        }
        else if(key == "OFF"){
            Serial.println("@@@@@ OFF");
            switchcase_set(OFF, it->value);
        }
        else
            Serial.println("@@@@@ INVALID KEY");
    }
}

/*--------------------------- JSON PARSING END -----------------------------*/


/*--------------------------- MQTT START ---------------------------*/

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
/*--------------------------- MQTT END ---------------------------------*/

/*---------------------------WEB SOCKET START ---------------------------*/

void websocket_onConnection(WebSocket& socket)
{
	websocket_nTotalActiveSockets++;

	// Notify everybody about new connection
	WebSocketsList &clients = xWSServer.getActiveWebSockets();
	for (int i = 0; i < clients.count(); i++)
		clients[i].sendString("New friend arrived! Total: " + String(websocket_nTotalActiveSockets));
}

/* EXAMPLES: install chrome websocket plugin and send
 * 1. {"switchboard":{"ON": 1}}
 * 2. {"switchboard":{"ON": 2}}
 * 3. {"switchboard":{"ON": 4}}
 * 4. {"switchboard":{"OFF": 7}}
 */
void websocket_onMessageReceive(WebSocket& socket, const String& message)
{
	Serial.printf("WebSocket message received:\r\n%s\r\n", message.c_str());
	String response = "Echo: " + message;
	socket.sendString(response);

    json_parseAndExecute(message);
}

void websocket_onDisconnect(WebSocket& socket)
{
	websocket_nTotalActiveSockets--;

	// Notify everybody about lost connection
	WebSocketsList &clients = xWSServer.getActiveWebSockets();
	for (int i = 0; i < clients.count(); i++)
		clients[i].sendString("We lost our friend :( Total: " + String(websocket_nTotalActiveSockets));
}

void websocket_onIndex(HttpRequest &request, HttpResponse &response)
{
	Serial.println("On index");
}


void websocket_startServer(){
    xWSServer.listen(80);
	xWSServer.addPath("/", websocket_onIndex);
	//server.setDefaultHandler(onFile);

	// Web Sockets configuration
	xWSServer.enableWebSockets(true);
	xWSServer.setWebSocketConnectionHandler(websocket_onConnection);
	xWSServer.setWebSocketMessageHandler(websocket_onMessageReceive);
    xWSServer.setWebSocketDisconnectionHandler(websocket_onDisconnect);

	Serial.println("\r\n=== WEB SERVER STARTED ===");
	Serial.println(WifiStation.getIP());
	Serial.println("==============================\r\n");

}

/*---------------------------WEB SOCKET END -----------------------------*/





// Will be called when WiFi station was connected to AP
void wifi_onConnectionSuccess()
{
    Serial.println("I'm CONNECTED");

    // Run MQTT client
    mqtt_startClient();

    // Run WebSocket Server
    websocket_startServer();

    // Start publishing loop
    //procTimer.initializeMs(20 * 1000, publishMessage).start(); // every 20 seconds
}

// Will be called when WiFi station timeout was reached
void wifi_onConnectionFail()
{
    Serial.println("I'm NOT CONNECTED. Need help :(");

    // .. some you code for device configuration ..
}

void system_onReady(){
    WifiStation.config(WIFI_SSID, WIFI_PWD);
    WifiStation.enable(true);
    WifiAccessPoint.enable(false);

    // Run our method when station was connected to AP (or not connected)
    WifiStation.waitConnection(wifi_onConnectionSuccess, 20, wifi_onConnectionFail);
    // We recommend 20+ seconds for connection timeout at start
}

void init()
{
    pinMode(R, OUTPUT);
    pinMode(G, OUTPUT);
    pinMode(B, OUTPUT);

    Serial.begin(SERIAL_BAUD_RATE); // 115200 by default
    Serial.systemDebugOutput(true); // Debug output to serial

    System.onReady(system_onReady);
}
