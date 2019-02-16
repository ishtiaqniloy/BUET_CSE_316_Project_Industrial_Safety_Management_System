#define senderID 2

int rd = 0;
char text[50];
int ti=0;

#include <SPI.h>
#include <stdlib.h>
#include <string.h>

#include "nRF24L01.h"
#include "RF24.h"

RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00100";

void setup()

{

                Serial.begin(9600);//serial data rate is set for 9600BPS

                pinMode(0,INPUT);//RXD pin is set for INPUT

                pinMode(1,OUTPUT);

                pinMode(4,OUTPUT);

                pinMode(7,OUTPUT);//PIN1,PIN7 are set for output

                radio.begin();
                radio.openWritingPipe(address);
                radio.setPALevel(RF24_PA_MIN);
                radio.stopListening();

               

}

void loop()

{
                  

                if (Serial.available() > 0) //if data received is available

                {       

                        rd = Serial.read();//read serial data available

                        if(rd=='t'){
                              ti=0;
                              
                              text[ti] = senderID + '0';
                              ti++;
                              
                              text[ti] = 't';
                              ti++;
                                        
                              while(1){
                                 if (Serial.available() > 0){
                                    rd = Serial.read();
                                    if(rd=='$')
                                      break;
                                    text[ti++] = rd;
                                 }
                              }

                        }

                        
                                   

                        if(rd=='g'){
                              
                              text[ti] = 'g';
                              ti++;          
                              while(1){
                                 if (Serial.available() > 0){
                                    rd = Serial.read();
                                    if(rd=='$')
                                      break;
                                    text[ti++] = rd;
                                 }
                              }

                              text[ti]=0;
                        }

                         
                        //itoa(receiveddata, text, 10);
                        
                        Serial.println(text);
                        radio.write(&text, sizeof(text));

//                        
                        
                        
                        
                          
                }
                //Serial.println("Hello");


}
