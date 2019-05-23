#ifndef USB_SERIAL_H
#define USB_SERIAL_H

#include "usbd_cdc_if.h"
#include <string.h>
#include <stdbool.h>

static bool b_message_received = false;
static uint32_t usb_last_message_len;
static uint8_t * usb_last_message_buff;

void usb_print_message(char * message);
void _usb_read_message(uint8_t* Buf, uint32_t Len);
bool usb_read_message(char * message, uint32_t size);	

#endif /* USB_SERIAL_H */
