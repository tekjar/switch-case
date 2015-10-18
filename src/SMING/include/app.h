#ifndef _APP_H
#define _APP_H

#include <user_config.h>
#include <SmingCore/SmingCore.h>

#ifdef __cplusplus
extern "C" {
#endif

extern String MAC_ADDR;

/*@@@@@@@@@@@@@@@ DEBUG @@@@@@@@@@@@@@@@@@@@@@@@@@@*/

#include <stdio.h>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define RESET "\033[0m"

#define DEBUG



#ifdef DEBUG
#define PRINTF_INFO(fmt, args...) Serial.printf(KBLU "{%s,%d} -> " fmt RESET, __func__, __LINE__, ##args)
#define PRINTF_DBG(fmt, args...)  Serial.printf(KYEL "{%s,%d} -> " fmt RESET, __func__, __LINE__, ##args) // ## removes trailing , when there is just string
#define PRINTF_ERR(fmt, args...)  Serial.printf(KCYN "{%s,%d} -> " fmt RESET, __func__, __LINE__, ##args) 
#define PRINTF_CRIT(fmt, args...) Serial.printf(KRED "{%s,%d} -> " fmt RESET, __func__, __LINE__, ##args)
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
//#define MQTT_BROKER "192.168.0.134"
#define MQTT_BROKER "test.mosquitto.org"
#define MQTT_PORT 1883

#define DISCOVERY_REQUEST_TOPIC  "switch-case/find-req"
#define DISCOVERY_REPLY_TOPIC    "switch-case/find-rep"
#define CONTROL_REQUEST_TOPIC    "switch-case/" + MAC_ADDR + "/ctrl-req" 
#define CONTROL_REPLY_TOPIC      "switch-case/" + MAC_ADDR + "/ctrl-rep"
#define OTA_REQUEST_TOPIC		 "switch-case/ota-req"
#define OTA_REPLY_TOPIC			 "switch-case/ota-rep"


void mqtt_publishMessage(String topic, String message);
void mqtt_onMessageReceive(String topic, String message);
void mqtt_startClient();

/*@@@@@@@@@@@@@@@@@@@@@ OTA @@@@@@@@@@@@@@@@@@@@@@@*/

void ota_onUpdate(bool result);
void ota_update();

/*@@@@@@@@@@@@@@@@@@@ SYSTEM @@@@@@@@@@@@@@@@@@@@@@@*/
void system_showInfo();





#ifdef __cplusplus
}
#endif

#endif //_APP_H
