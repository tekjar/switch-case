
#include <app.h>

void switchcase_discover(){
	String mac = WifiStation.getMAC();
	/* Construct a proper json message with mac address */
	//TBI
	/*****/
	mqtt_publishMessage(DISCOVERY_REPLY_TOPIC, mac);
}



