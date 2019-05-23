#include "usb_serial.h"

void usb_print_message(char * message)
{
	int i = 0;
	for (i = 0; i < 100; i++)
	{
		uint8_t status = CDC_Transmit_FS((uint8_t *)message, strlen(message));
		if (status == USBD_BUSY)
		{
			HAL_Delay(1);			
		}
		else
		{
			break;
		}
	}
}


void _usb_read_message(uint8_t* Buf, uint32_t Len)
{
	b_message_received = true;
	usb_last_message_buff = Buf;
	usb_last_message_len = Len;
}

bool usb_read_message(char * message, uint32_t size)
{
	int i;
	for (i = 0; i < 100; i++)
	{
		if (b_message_received)
		{
			uint32_t max_size = usb_last_message_len;
			if (max_size > size)
			{
				max_size = size;
			}
			b_message_received = false;
			strncpy(message, (char *) usb_last_message_buff, max_size);
			return true;
		}
		HAL_Delay(10);
	}
	return false;
}
