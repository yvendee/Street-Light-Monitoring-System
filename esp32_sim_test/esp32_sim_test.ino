#include "gprs.h"

//  ESP32GPIO pins 16 (RX) ---> SIM800 TXD
// ESP32GPIO pins 17 (TX)  ---> SIM800 RXD
// ESP32GPIO (GND)  ---> SIM800 GND
// ESP32GPIO (+5V)  ---> SIM800 5V-4V

GPRS gprs;

void setup() {
  Serial.begin(9600);
  while(!Serial);
  Serial.println("GPRS - Send SMS Test ...");
  gprs.preInit();
  delay(1000);
  while(0 != gprs.init()) {
      delay(1000);
      Serial.print("init error\r\n");
  }  
  Serial.println("Init success, start to send SMS message...");
  gprs.sendSMS("130****3364","hello,world"); //define phone number and text
}

void loop() {
  //nothing to do
}
