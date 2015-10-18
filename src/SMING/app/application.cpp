#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include <app.h>

/**
 * @file
 * @brief Where it all starts
 */

// If you want, you can define WiFi settings globally in Eclipse Environment Variables
#ifndef WIFI_SSID
#define WIFI_SSID "OpenWrt" // Put you SSID and Password here
#define WIFI_PWD "abc@12345"
#endif

String MAC_ADDR;

// Forward declarations
void mqtt_startClient();
void mqtt_onMessageReceive(String topic, String message);


Timer procTimer;

/**
 * @brief Callback when wifi connection is successful
 * @details All your mqtt services will be started at this point
 */
//TODO: Handling wifi disconnections 
void wifi_onConnectionSuccess()
{
    PRINTF_DBG("Connected to AP\n");

    MAC_ADDR = WifiStation.getMAC();

    PRINTF_DBG("Switch Board Mac Address = %s\n", MAC_ADDR.c_str()); 

    // Run MQTT client
    mqtt_startClient();    
}

// Will be called when WiFi station timeout was reached
void wifi_onConnectionFail()
{
    PRINTF_CRIT("I'm NOT CONNECTED. Need help :(");

    // .. some you code for device configuration ..
}

/**
 * @brief callback when system is ready
 * @details 
 */
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
