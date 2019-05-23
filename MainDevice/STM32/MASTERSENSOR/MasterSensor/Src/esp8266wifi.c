#include "esp8266wifi.h"


HAL_StatusTypeDef esp8266_wifi_send_string(UART_HandleTypeDef * huart_handle, char * data)
{
	HAL_StatusTypeDef status = HAL_UART_Transmit(huart_handle, (uint8_t *) data, strlen((char *) data), 1000); 
	usb_print_message(">>");
	char data_len [24];
	snprintf(data_len, 23, "LENGTH: %d -> ", strlen((char *) data));
	usb_print_message(data_len);
	usb_print_message(data);
	return status;
}

bool esp8266_wifi_init(UART_HandleTypeDef * huart_handle)
{
	if (!esp8266_wifi_wait_ok(huart_handle))
	{
		usb_print_message("Inicio: no se recibio ok\r\n");
		return false;
	}
	char message [126];
	if (esp8266_wifi_send_string(huart_handle, "AT+RST\r\n") != HAL_OK)
	{
		usb_print_message("Inicio: no se envio\r\n");
		return false;
	}
	if (esp8266_wifi_read_string(huart_handle, message, 125) != HAL_OK)
	{
		usb_print_message("Inicio: no se pudo leer\r\n");
		return false;
	}
	if (!esp8266_set_mode(huart_handle, 2))
	{
		return false;
	}
	if (esp8266_wifi_send_string(huart_handle, "AT+CIPMUX=0\r\n") != HAL_OK)
	{
		usb_print_message("Inicio: no se envia unica conexion\r\n");
		return false;
	}
	if (esp8266_wifi_read_string(huart_handle, message, 125) != HAL_OK)
	{
		usb_print_message("Inicio: no se pudo leer\r\n");
		return false;
	}
	return true;
}

bool esp8266_wifi_restart(UART_HandleTypeDef * huart_handle)
{
	return esp8266_wifi_init(huart_handle) == HAL_OK;
}

bool esp8266_disconnect(UART_HandleTypeDef * huart_handle)
{
	char message [126];
	if (esp8266_wifi_send_string(huart_handle, "AT+CWQAP\r\n") != HAL_OK)
	{
		usb_print_message("Desconectar: no se pudo enviar\r\n");
		return false;
	}
	if (esp8266_wifi_read_string(huart_handle, message, 125) != HAL_OK)
	{
		usb_print_message("Desconectar: no se pudo leer\r\n");
		return false;
	}
	return true;
}

bool esp8266_connect(UART_HandleTypeDef * huart_handle, char * ssid, char * password)
{	
	char message[526];
	if (!esp8266_disconnect(huart_handle))
	{
		return false;
	}
	esp8266_wifi_send_string(huart_handle, "AT+CWLAP\r\n");
	HAL_Delay(2000);
	esp8266_wifi_read_string(huart_handle, message, 525);
	strcpy(message, "AT+CWJAP=\"");
	strcat(message, ssid);
	strcat(message, "\",\"");
	strcat(message, password);
	strcat(message, "\"\r\n");
	if (esp8266_wifi_send_string(huart_handle, message) != HAL_OK)
	{
		usb_print_message("Conectar: no se pudo enviar\r\n");
		return false;
	}
	HAL_Delay(5000);
	esp8266_wifi_read_string(huart_handle, message, 525);
	return true;
}

bool esp8266_ip(UART_HandleTypeDef * huart_handle, char * message)
{	
	if (esp8266_wifi_send_string(huart_handle, "AT+CIFSR\r\n") != HAL_OK)
	{
		usb_print_message("IP: no se pudo enviar\r\n");
		return false;
	}
	if (esp8266_wifi_read_string(huart_handle, message, 256) != HAL_OK)
	{
		usb_print_message("IP: no se pudo enviar\r\n");
		return false;
	}
	return true;
}

bool esp8266_wifi_read_string(UART_HandleTypeDef * huart_handle, char * message, uint16_t size)
{
	memset(message, 0, size);
	message[0] = '\0';
	message[size - 1] = '\0';
	int i;
	uint8_t received_byte;
	for (i = 0; i < size - 1; i++)
	{
		HAL_StatusTypeDef status = HAL_UART_Receive(huart_handle, &received_byte, 1, 1000);
		if (status == HAL_OK)
		{
			message[i] = received_byte;
		}
		else
		{
			message[i] = '\0';
			break;
		}
	}
	if (i == 0)
	{
		return false;
	}
	usb_print_message(message);
	usb_print_message("\r\n");
	return true;
}

bool esp8266_set_mode(UART_HandleTypeDef * huart_handle, int mode)
{
	if (mode >= 1 && mode <= 3)
	{
		char message [126];
		snprintf(message, 14, "AT+CWMODE=%d\r\n", mode);
		if (esp8266_wifi_send_string(huart_handle, message) != HAL_OK)
		{
			usb_print_message("Modo: no se pudo enviar\r\n");
			return false;
		}
		if (esp8266_wifi_read_string(huart_handle, message, 125) != HAL_OK)
		{
			usb_print_message("Modo: no se pudo leer\r\n");
			return false;
		}
		return true;
	}
	return false;
}

bool esp8266_wifi_wait_ok(UART_HandleTypeDef * huart_handle)
{
	int i;
	char message [32];
	for(i = 0; i < 5; i++)
	{
		esp8266_wifi_send_string(huart_handle, "AT\r\n");
		if (esp8266_wifi_read_string(huart_handle, message, 32))
		{
			if (strstr(message, "OK") != NULL)
			{
				return true;
			}
			else
			{
				usb_print_message(message);
				usb_print_message("\r\n");
			}
		}
		HAL_Delay(1000);
	}
	return false;
}

