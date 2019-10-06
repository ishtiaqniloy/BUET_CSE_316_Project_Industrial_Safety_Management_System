/*
* Arduino Wireless Communication Tutorial
*     Example 1 - Transmitter Code
*                
* by Dejan Nedelkovski, www.HowToMechatronics.com
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/

#define senderID 1

#include <SPI.h>
#include <stdlib.h>
#include <string.h>

#include "nRF24L01.h"
#include "RF24.h"

RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00100";
void setup() {
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}
void loop() {
  char text[20]; 
  //itoa(senderID, text, 10);
  strcpy(text, "Hello World");
  radio.write(&text, sizeof(text));
  delay(1000);
}


