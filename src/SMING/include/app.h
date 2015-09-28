#ifndef _APP_H
#define _APP_H

#include <user_config.h>
#include <SmingCore/SmingCore.h>

#ifdef __cplusplus
extern "C" {
#endif

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
#define DISCOVERY_REQUEST_TOPIC        "discovery-request"
#define DISCOVERY_REPLY_TOPIC          "discovery-reply"
#define CONTROL_REQUEST_TOPIC          "control-request"
#define CONTROL_REPLY_TOPIC            "control-reply"

void mqtt_publishMessage(String topic, String message);
void mqtt_onMessageReceive(String topic, String message);
void mqtt_startClient();

#ifdef __cplusplus
}
#endif

#endif //_APP_H
