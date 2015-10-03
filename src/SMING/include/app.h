#ifndef _APP_H
#define _APP_H

#include <user_config.h>
#include <SmingCore/SmingCore.h>

#ifdef __cplusplus
extern "C" {
#endif

extern String MAC_ADDR;


/*@@@@@@@@@@@@@@@ PARSE AND CONTROL @@@@@@@@@@@@@@@*/
#define R 12
#define G 13
#define B 14


#define SWITCH_1 (1 << 0)
#define SWITCH_2 (1 << 1)
#define SWITCH_3 (1 << 2)


typedef enum{
    ON,
    OFF
}COMMAND;


void json_parseAndExecute(String message);
void switchcase_set(COMMAND command, int value);


/*@@@@@@@@@@@@@@@ MQTT @@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
//#define MQTT_BROKER "192.168.0.134"
#define MQTT_BROKER "test.mosquitto.org"
#define MQTT_PORT 1883

#define DISCOVERY_REQUEST_TOPIC  "switch-case/find-req"
#define DISCOVERY_REPLY_TOPIC    "switch-case/find-rep"
#define CONTROL_REQUEST_TOPIC    "switch-case/" + MAC_ADDR + "/ctrl-req" 
#define CONTROL_REPLY_TOPIC      "switch-case/" + MAC_ADDR + "/ctrl-rep"

void mqtt_publishMessage(String topic, String message);
void mqtt_onMessageReceive(String topic, String message);
void mqtt_startClient();

#ifdef __cplusplus
}
#endif

#endif //_APP_H
