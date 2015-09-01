#include "ets_sys.h"
#include "driver/uart.h"
#include "osapi.h"
#include "mqtt.h"
#include "wifi.h"
#include "config.h"
#include "debug.h"
#include "gpio.h"
#include "user_interface.h"
#include "mem.h"

#define RED 1

#ifdef RED
#define pin GPIO_ID_PIN(12)
#define pin_func FUNC_GPIO12
#define pin_mux PERIPHS_IO_MUX_MTDI_U
#elif BLUE
#define pin GPIO_ID_PIN(13)
#define pin_func FUNC_GPIO13
#define pin_mux PERIPHS_IO_MUX_MTCK_U
#elif GREEN
#define pin GPIO_ID_PIN(14)
#define pin_func FUNC_GPIO14
#define pin_mux PERIPHS_IO_MUX_MTMS_U
#endif

#define LOW 0
#define HIGH 1


static volatile os_timer_t some_timer;



void some_timer_func(void *arg) // in Arduino this is loop the main loop
{
	//Do blinky stuff
	if GPIO_INPUT_GET(pin)
	{
		//Set led to LOW
		GPIO_OUTPUT_SET(pin, LOW);
		os_printf("ON \n\r");      // In Arduino this is Serial.println("ON");

	}
	else
	{
		//Set led to HIGH
		GPIO_OUTPUT_SET(pin, HIGH);
		os_printf("OFF \n\r");      // In Arduino this is Serial.println("OFF");

	}
}


MQTT_Client mqttClient;

void wifiConnectCb(uint8_t status)
{
	if(status == STATION_GOT_IP){
		MQTT_Connect(&mqttClient);
	} else {
		MQTT_Disconnect(&mqttClient);
	}
}
void mqttConnectedCb(uint32_t *args)
{
	MQTT_Client* client = (MQTT_Client*)args;
	os_printf("MQTT: Connected\r\n");
	MQTT_Subscribe(client, "/mqtt/topic/0", 0);
	MQTT_Subscribe(client, "/mqtt/topic/1", 1);
	MQTT_Subscribe(client, "/mqtt/topic/2", 2);

	MQTT_Publish(client, "/mqtt/topic/0", "hello0", 6, 0, 0);
	MQTT_Publish(client, "/mqtt/topic/1", "hello1", 6, 1, 0);
	MQTT_Publish(client, "/mqtt/topic/2", "hello2", 6, 2, 0);

}

void mqttDisconnectedCb(uint32_t *args)
{
	MQTT_Client* client = (MQTT_Client*)args;
	os_printf("MQTT: Disconnected\r\n");
}

void mqttPublishedCb(uint32_t *args)
{
	MQTT_Client* client = (MQTT_Client*)args;
	os_printf("MQTT: Published\r\n");
}

void mqttDataCb(uint32_t *args, const char* topic, uint32_t topic_len, const char *data, uint32_t data_len)
{
	char *topicBuf = (char*)os_zalloc(topic_len+1),
	     *dataBuf = (char*)os_zalloc(data_len+1);

	MQTT_Client* client = (MQTT_Client*)args;

	os_memcpy(topicBuf, topic, topic_len);
	topicBuf[topic_len] = 0;

	os_memcpy(dataBuf, data, data_len);
	dataBuf[data_len] = 0;

	os_printf("Receive topic: %s, data: %s \r\n", topicBuf, dataBuf);
	os_free(topicBuf);
	os_free(dataBuf);
}


void ICACHE_FLASH_ATTR user_init()
{
	gpio_init();
	os_delay_us(1000000);
	uart_div_modify(0, UART_CLK_FREQ / 115200);
	os_delay_us(3000000);
	PIN_FUNC_SELECT(pin_mux,pin_func);
	GPIO_OUTPUT_SET(pin, 1);
	os_printf("Hello World, Blinking\n\r"); // In Arduino this is Serial.println("Hello World, Blinking");
	os_delay_us(1000000);
	os_printf("Hello World, Blinking\n\r"); // In Arduino this is Serial.println("Hello World, Blinking");
	os_delay_us(1000000);
	os_printf("Hello World, Blinking\n\r"); // In Arduino this is Serial.println("Hello World, Blinking");
	os_delay_us(1000000);
	os_printf("Hello World, Blinking\n\r"); // In Arduino this is Serial.println("Hello World, Blinking");
	os_timer_disarm(&some_timer);
	os_timer_setfn(&some_timer, (os_timer_func_t *)some_timer_func, NULL);
	os_timer_arm(&some_timer, 5000, 1);
#if 0
	CFG_Load();
	os_printf("Hello World MQTT\n\r");

	MQTT_InitConnection(&mqttClient, sysCfg.mqtt_host, sysCfg.mqtt_port, sysCfg.security);
	//MQTT_InitConnection(&mqttClient, "192.168.11.122", 1880, 0);

	MQTT_InitClient(&mqttClient, sysCfg.device_id, sysCfg.mqtt_user, sysCfg.mqtt_pass, sysCfg.mqtt_keepalive, 1);
	//MQTT_InitClient(&mqttClient, "client_id", "user", "pass", 120, 1);

	MQTT_InitLWT(&mqttClient, "/lwt", "offline", 0, 0);
	MQTT_OnConnected(&mqttClient, mqttConnectedCb);
	MQTT_OnDisconnected(&mqttClient, mqttDisconnectedCb);
	MQTT_OnPublished(&mqttClient, mqttPublishedCb);
	MQTT_OnData(&mqttClient, mqttDataCb);

	WIFI_Connect(sysCfg.sta_ssid, sysCfg.sta_pwd, wifiConnectCb);
#endif
	os_printf("\r\nSystem started ...\r\n");
}
