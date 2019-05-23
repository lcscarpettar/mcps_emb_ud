//#include "WiFi.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  /*
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  */

  Serial.println("Setup done");
  uint64_t chipid = ESP.getEfuseMac();
  unsigned long long1 = (unsigned long)((chipid & 0xFFFF0000) >> 16 );
  unsigned long long2 = (unsigned long)((chipid & 0x0000FFFF));
  String hex = String(long1, HEX) + String(long2, HEX); // six octets
  Serial.println(hex);
}

void loop() {
  // put your main code here, to run repeatedly:

}
