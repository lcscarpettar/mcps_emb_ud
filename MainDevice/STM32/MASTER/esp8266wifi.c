#include "esp8266wifi.h"


void esp8266_wifi_send_string(UART_HandleTypeDef * huart_handle, char * data)
{
	HAL_UART_Transmit(huart_handle, (uint8_t *) data, strlen((char *) data), 500); 
}

void esp8266_wifi_init(UART_HandleTypeDef * huart_handle)
{
	esp8266_wifi_send_string(huart_handle, "AT+RST\r\n");
	esp8266_wifi_read_string(huart_handle, 4);
	esp8266_set_mode(huart_handle, 2);
	esp8266_wifi_send_string(huart_handle, "AT+CIPMUX=0\r\n");	
	esp8266_wifi_read_string(huart_handle, 4);
}

void esp8266_wifi_restart(UART_HandleTypeDef * huart_handle)
{
	esp8266_wifi_init(huart_handle);
}

void esp8266_disconnect(UART_HandleTypeDef * huart_handle)
{
	esp8266_wifi_send_string(huart_handle, "AT+CWQAP\r\n");
	esp8266_wifi_read_string(huart_handle, 17);
}

void esp8266_connect(UART_HandleTypeDef * huart_handle, char * ssid, char * password)
{	
	esp8266_disconnect(huart_handle);
	char message[256];
	strcpy(message, "AT+CWJAP=\"");
	strcat(message, ssid);
	strcat(message, "\",\"");
	strcat(message, password);
	strcat(message, "\"\r\n");
	esp8266_wifi_send_string(huart_handle, message);
}

char * esp8266_ip(UART_HandleTypeDef * huart_handle)
{	
	esp8266_wifi_send_string(huart_handle, "AT+CIFSR\r\n");
	return esp8266_wifi_read_string(huart_handle, 256);
}

char * esp8266_wifi_read_string(UART_HandleTypeDef * huart_handle, uint16_t size)
{
	char * message;
	HAL_UART_Receive(huart_handle, (uint8_t *)message, size, 250);
	return message;
}

void esp8266_set_mode(UART_HandleTypeDef * huart_handle, int mode)
{
	if (mode >= 1 && mode <= 3)
	{
		char message [15];
		snprintf(message, 14, "AT+CWMODE=%d\r\n", mode);
		esp8266_wifi_send_string(huart_handle, message);
		esp8266_wifi_read_string(huart_handle, 4);
	}
}