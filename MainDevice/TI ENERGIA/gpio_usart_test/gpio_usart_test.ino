/*
  DigitalReadSerial with on-board Pushbutton
  Reads a digital input on pin 5, prints the result to the serial monitor 
 
  Harware Required:
  * MSP-EXP430G2 LaunchPad
  
  This example code is in the public domain.
 */

 #include "espwifi.h"

// digital pin 5 has a pushbutton attached to it. Give it a name:
int PUSH_BUTTON = P2_1;
int BOARD_RED_LED = P1_0;
int BOARD_GREEN_LED = P4_7;



// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  PCSerial.begin(9600); // msp430g2231 must use 4800
  WifiSerial.begin(115200);
  // make the on-board pushbutton's pin an input pullup:
  pinMode(PUSH_BUTTON, INPUT_PULLUP);
  pinMode(BOARD_RED_LED, OUTPUT);
  pinMode(BOARD_GREEN_LED, OUTPUT);
  digitalWrite(BOARD_RED_LED, HIGH);
  digitalWrite(BOARD_GREEN_LED, HIGH);
  char body [526];  
  WifiMessage wifiMcps;
  
}

// the loop routine runs over and over again forever:
void loop() {
  
  while (PCSerial.available() > 0)
  {
    WifiSerial.write(PCSerial.read());    
  }    
  while (WifiSerial.available() > 0)
  {
    PCSerial.write(WifiSerial.read());    
  }
}
