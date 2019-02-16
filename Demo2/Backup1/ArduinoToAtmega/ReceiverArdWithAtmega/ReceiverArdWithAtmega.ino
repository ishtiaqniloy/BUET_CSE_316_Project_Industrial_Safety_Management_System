/*
* Arduino Wireless Communication Tutorial
*       Example 1 - Receiver Code
*                
* by Dejan Nedelkovski, www.HowToMechatronics.com
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00100";
void setup() {
  Serial.begin(9600);

       //pinMode(0,INPUT);//RXD pin is set for INPUT
       pinMode(1,OUTPUT);
       pinMode(4,OUTPUT);
       pinMode(7,OUTPUT);//PIN1,PIN7 are set for output

  
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    
    char text[32] = "2t65g123";
    radio.read(&text, sizeof(text));

   //Serial.println(text);  

    Serial.write('m');
    delay(20);
    
    int txtLen = strlen(text);
    for(int i=0; i<txtLen; i++){
      Serial.write(text[i]);
      delay(20);
    }
    Serial.write('\n');
    delay(450);

      
    
    //int moduleNo;
    //int temp;
    //int mq;
    //sscanf(text, "%dt%dg%d", &moduleNo, &temp, &mq);
    
//    Serial.println(moduleNo);
//    Serial.println(temp);
//    Serial.println(mq);
//    delay(2000);


    
  }
}
