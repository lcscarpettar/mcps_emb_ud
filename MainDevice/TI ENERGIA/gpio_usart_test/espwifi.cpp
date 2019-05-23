#include "espwifi.h"


void espwifi_sendATCommand(char * command)
{
  strcat(command, "\r\n");
  WifiSerial.write((uint8_t *) command, strlen(command));
  PCSerial.write((uint8_t *) command, strlen(command));
}

size_t espwifi_readResponse(char * buff, size_t len)
{
  strcpy(buff, "");
  size_t i = 0;
  char newByte [2];
  newByte[1] = '\0';
  while(WifiSerial.available() > 0)
  {
    newByte[0] = WifiSerial.read();
    strcat(buff, newByte);
    i++;
    if (i > len)
    {
      break;
    }
  }
  PCSerial.write((uint8_t*) buff, strlen(buff));
  return i;
}

void espwifi_init(void)
{
  char ssid [] = STASSID;
  char password [] = STAPSK;
  char wifi_message[124];
  strcpy(wifi_message, "AT+RST");
  espwifi_sendATCommand(wifi_message);
  delay(2000);
  espwifi_readResponse(wifi_message, 120);
  if(strstr(wifi_message, "OK") == NULL)
  {
    PCSerial.println("Error leyendo Modulo ESP8266");
    return;
  }
  sprintf(wifi_message, "AT+CWJAP=\"%s\",\"%s\"", ssid, password);
  espwifi_sendATCommand(wifi_message);
  delay(5000);
  espwifi_readResponse(wifi_message, 120);
  strcpy(wifi_message, "AT+CIPMODE=0");
  espwifi_sendATCommand(wifi_message);
  delay(9000);
  strcpy(wifi_message, "AT+CIPMUX=0");
  espwifi_sendATCommand(wifi_message);
  delay(4000);
  espwifi_readResponse(wifi_message, 120);
}

void espwifi_sendHttp(WifiMessage * message)
{
  // se prepara el mensaje
  char buff[2048];
  char command[526];
  size_t headerLines = 1;
  if (message->method == HTTP_GET || message->method == HTTPS_GET)
  {
    sprintf(buff, "GET %s HTTP/1.1\nHost: ", message->parameters);
  }
  else
  {
    sprintf(buff, "POST %s HTTP/1.1\nHost: ", message->parameters);
  }
  strcat(buff, message->url);
  if (message->method == HTTP_POST || message->method == HTTPS_POST)
  {
    strcat(buff, "\nContent-Type: application/json\n");
    strcat(buff, "User-Agent: PostmanRuntime/7.13.0\n");
    strcat(buff, "accept-encoding: gzip, deflate\n");
    strcat(buff, "Connection: keep-alive\n");
    strcat(buff, "content-length: ");
    char buff_num[9];
    sprintf(buff_num, "%d", strlen(message->body));
    strcat(buff, buff_num);
    headerLines += 4;
  }
  strcat(buff, "\ncache-control: no-cache\n");
  headerLines++;
  if (message->method == HTTP_POST || message->method == HTTPS_POST)
  {
    strcat(buff, "\n");
    strcat(buff, message->body);
    headerLines += 2;
  }
  // empiezan comandos
  sprintf(command, "AT+CIPSTART=4,\"TCP\",\"%s\",", message->url);
  if (message->method == HTTP_GET || message->method == HTTP_POST)
  {
    strcat(command, "80");
  }
  else
  {
    strcat(command, "443");
  }
  espwifi_sendATCommand(command);
  delay(100);
  headerLines++;
  sprintf(command, "AT+CIPSEND=4,%d", strlen(buff) + headerLines);
  espwifi_sendATCommand(command);
  delay(100);
  espwifi_sendATCommand(buff);
}
