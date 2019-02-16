//pump A-0, B-1
//valve C-1, D-0
//fan A-1 B-0


#ifndef F_CPU
#define F_CPU 1000000 // 1 MHz clock speed
#endif

#define vccVal 4.82

#define mq2Threshold 250
#define grvThreshold 400

#define led 0
#define buzzer 1
#define fan 3
#define pump 4
#define valve 5
#define powerLine 6


#define D4 eS_PORTD4
#define D5 eS_PORTD5
#define D6 eS_PORTD6
#define D7 eS_PORTD7
#define RS eS_PORTC6
#define EN eS_PORTC7

#include<avr/io.h>
#include<avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lcd.h"
#include "ftoa.h"
//using namespace std;

unsigned char receiveBuff[150];
int receiveIdx = 0;

int temparature1 = 0;  //module1
char temparatureStr1[20];

int temparature2 = 0;  //module2
char temparatureStr2[20];

int mq2Val1 = 0;			//module1
char mq2Str1[20];

int mq2Val2 = 0;			//module2
char mq2Str2[20];


unsigned char recieveChar(){

	// Wait until RXC flag is set to logic 1
	while ((UCSRA & (1<<RXC)) == 0x00);
	return UDR; // Read the received character from UDR

}

ISR(USART_RXC_vect){
	unsigned char ch = recieveChar();

	receiveBuff[receiveIdx] = ch;
	receiveIdx++;


	if(ch=='\n' && receiveBuff[0]=='m'){
		int moduleNo;

		receiveBuff[receiveIdx-1] = '\0';
	
		if(receiveBuff[1]=='1'){
			sscanf(receiveBuff, "m%dt%dg%d", &moduleNo, &temparature1, &mq2Val1);

			char dummy[50];

     		itoa(temparature1, temparatureStr1, 10 );
			itoa(mq2Val1, mq2Str1, 10);

			itoa( temparature2, temparatureStr2, 10 );
			itoa(mq2Val2, mq2Str2, 10);

			strcpy(dummy, "1:T=");
			strcat(dummy, temparatureStr1);
			strcat(dummy, " M=");
			strcat(dummy, mq2Str1);
			strcat(dummy, "    ");
			
			Lcd4_Clear();
			Lcd4_Set_Cursor(1,0);
			Lcd4_Write_String(dummy);

			strcpy(dummy, "2:T=");
			strcat(dummy, temparatureStr2);
			strcat(dummy, " M=");
			strcat(dummy, mq2Str2);
			strcat(dummy, "    ");
			Lcd4_Set_Cursor(2,0);
			Lcd4_Write_String(dummy);

			_delay_ms(50);

			PORTB = PORTB ^ 1;	//PB0

		}
		else if(receiveBuff[1]=='2'){
			sscanf(receiveBuff, "m%dt%dg%d", &moduleNo, &temparature2, &mq2Val2);
			
			char dummy[50];
			
			itoa(temparature1, temparatureStr1, 10 );
			itoa(mq2Val1, mq2Str1, 10);

     		itoa( temparature2, temparatureStr2, 10 );
			itoa(mq2Val2, mq2Str2, 10);
	
			strcpy(dummy, "1:T=");
			strcat(dummy, temparatureStr1);
			strcat(dummy, " M=");
			strcat(dummy, mq2Str1);
			strcat(dummy, "    ");
			
			Lcd4_Clear();
			Lcd4_Set_Cursor(1,0);
			Lcd4_Write_String(dummy);

			strcpy(dummy, "2:T=");
			strcat(dummy, temparatureStr2);
			strcat(dummy, " M=");
			strcat(dummy, mq2Str2);
			strcat(dummy, "    ");
			Lcd4_Set_Cursor(2,0);
			Lcd4_Write_String(dummy);
			
			_delay_ms(50);

			PORTB = PORTB ^ 2;	//PB1

		}

		if(mq2Val1>mq2Threshold){	//PB2
			PORTB |=4;
			_delay_ms(200);
		}
		else{
			PORTB &= ~(4);
		}

		if(mq2Val2>mq2Threshold){ //PB3
			PORTB |=8;
			_delay_ms(200);
		}
		else{
			PORTB &= ~(8);
		}

		receiveIdx = 0;
	}
	else if(ch=='\n'){
		receiveIdx = 0;
		PORTB ^=16;

	}



}

int main(void)

{   

	DDRD = 0xFF;	//for LCD and ADC
	DDRC = 0xFF;

	Lcd4_Init();

	//USART Init below:
    DDRD |= 1 << PIND1;//pin1 of portD as OUTPUT ?
    DDRD &= ~(1 << PIND0);//pin0 of portD as INPUT ?
    PORTD |= 1 << PIND0;
    int UBBRValue = 12;//AS described before setting baud rate 9600BPS, double transmission
    UBRRH = (unsigned char) (UBBRValue >> 8);
    UBRRL = (unsigned char) UBBRValue;
    UCSRA = 2;
    UCSRB = (1<<RXCIE) | (1 << RXEN) | (1 << TXEN);    //Enable the receiver and transmitter and Recieve Interrupt
    UCSRC = 0b10000110;

    DDRB = 0xFF;			//?????
	PORTB = 0;


	//string init
	receiveBuff[0] = 0;
	temparatureStr1[0] = 0;
	temparatureStr2[0] = 0;
	mq2Str1[0] = 0;
	mq2Str2[0] = 0;

	sei();

	while(1){
		//do nthing
		//everything is done with interrrupt
	}

    

}

 
