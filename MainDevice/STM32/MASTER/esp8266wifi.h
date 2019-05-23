#ifndef ESP8266WIFI_H
#define ESP8266WIFI_H

#include "stm32f1xx_hal.h"
#include <string>


void esp8266_wifi_init(UART_HandleTypeDef * huart_handle);
void esp8266_wifi_restart(UART_HandleTypeDef * huart_handle);
void esp8266_wifi_send_string(UART_HandleTypeDef * huart_handle, char * data);
int esp8266_is_started(UART_HandleTypeDef * huart_handle);
void esp8266_set_mode(UART_HandleTypeDef * huart_handle, int mode);
void esp8266_connect(UART_HandleTypeDef * huart_handle, char * ssid, char * password);
void esp8266_disconnect(UART_HandleTypeDef * huart_handle);
char * esp8266_ip(UART_HandleTypeDef * huart_handle);
char * esp8266_wifi_read_string(UART_HandleTypeDef * huart_handle, uint16_t size);
	
#endif /* ESP8266WIFI_H */
