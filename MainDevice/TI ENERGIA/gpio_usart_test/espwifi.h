#ifndef ESPWIFI_H
#define ESPWIFI_H

#include <stdio.h>
#include <string.h>
#include <Arduino.h>
#include <HardwareSerial.h>


#ifndef STASSID
  #define STASSID "your-ssid"
  #define STAPSK  "your-password"
#endif

#define WifiSerial Serial1
#define PCSerial Serial


typedef enum {HTTP_POST, HTTP_GET, HTTPS_GET, HTTPS_POST} HTTP_METHOD;

typedef struct _WifiMessage
{
  char * url;
  char * parameters;
  HTTP_METHOD method;
  char * body;
} WifiMessage;

void espwifi_sendATCommand(char * command);
size_t espwifi_readResponse(char * buff, size_t len);
void espwifi_init(void);
void espwifi_sendHttp(WifiMessage * message);


#endif /* ESPWIFI_H */
