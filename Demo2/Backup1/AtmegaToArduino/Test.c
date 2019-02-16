
#define F_CPU 1000000UL
#define vccVal 4.82
 
#include<avr/io.h>
#include<avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ftoa.h"

char str[100];
int flag = 0;

float temparature = 0;  //adc0
int tempInt = 0;
char temparatureStr[20];

int mq2Val = 0;         //adc1
char mq2Str[20];

float stepSize = vccVal/1024;
float TempStepSize=(vccVal*1000)/(1024*10); //for first way of temparature

ISR(ADC_vect){
    uint16_t valLO = ADCL;
    uint16_t valHI = ADCH;

    uint16_t adcVal = valLO|(valHI<<8);
    

    if(flag==0){        //temparature
        ADMUX|=0b00000001;
        flag=1;

        temparature = adcVal*TempStepSize;
        tempInt = (int) temparature;
        itoa(tempInt, temparatureStr, 10);
        //ftoa(temparature,temparatureStr,2);
    }
    else if(flag==1){   //mq2
        ADMUX&=0b11111100;
        flag=0;

        mq2Val = adcVal;
        itoa(mq2Val, mq2Str, 10);


    }
    ADCSRA |= (1<<ADSC);
} 

void sendChar(unsigned char c){
    while (! (UCSRA & (1 << UDRE)) );
                        
    UDR = c;//once transmitter is ready sent eight bit data

    _delay_ms(20);
}

int main(void)

{   

    //USART Init below:
    DDRD |= 1 << PIND1;//pin1 of portD as OUTPUT
    DDRD &= ~(1 << PIND0);//pin0 of portD as INPUT
    PORTD |= 1 << PIND0;
    int UBBRValue = 12;//AS described before setting baud rate 9600BPS, double transmission
    UBRRH = (unsigned char) (UBBRValue >> 8);
    UBRRL = (unsigned char) UBBRValue;
    UCSRA = 2;
    UCSRB = (1 << RXEN) | (1 << TXEN);    //Enable the receiver and transmitter
    UCSRC = 0b10000110;
    
    //USART Init below:
    ADMUX = 0b01000000;     //for adc
    ADCSRA = 0b10001101;

    sei();

    ADCSRA |= (1<<ADSC);    //start ADC
    
    char tstr [20];
    char gstr [20];
    int tlen;
    int glen;

    while (1)

    {   

        
        strcpy(tstr, temparatureStr);
        tlen = strlen(tstr);

        strcpy(gstr, mq2Str);
        glen = strlen(gstr);

        sendChar('t');
        for(int i=0; i<tlen; i++){
            sendChar(tstr[i]);
        }
        sendChar('$');


        sendChar('g');
        for(int i=0; i<glen; i++){
            sendChar(gstr[i]);
        }
        sendChar('$');
      
            

    }

}

 