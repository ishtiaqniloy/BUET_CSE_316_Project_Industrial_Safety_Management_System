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

char str[100];
int flag = 0;

float temparature0 = 0;	//adc0
char temparatureStr0[20];

float temparature1 = 0;  //module1
char temparatureStr1[20];

float temparature2 = 0;  //module2
char temparatureStr2[20];

int mq2Val0 = 0;			//adc1
char mq2Str0[20];

int mq2Val1 = 0;			//module1
char mq2Str1[20];

int mq2Val2 = 0;			//module2
char mq2Str2[20];

int grvVal = 0;		//adc2
char grvStr[20];

float stepSize = vccVal/1024;
float TempStepSize=(vccVal*1000)/(1024*10);	//for first way of temparature

ISR(ADC_vect){
	uint16_t valLO = ADCL;
	uint16_t valHI = ADCH;

	uint16_t adcVal = valLO|(valHI<<8);
	

	if(flag==0){		//temparature
		ADMUX|=0b00000001;
		flag=1;

		temparature0 = adcVal*TempStepSize;
		ftoa(temparature0,temparatureStr0,2);
	}
	else if(flag==1){	//mq2
		ADMUX&=0b11111110;
		ADMUX|=0b00000010;
		flag=2;

		mq2Val0 = adcVal;
		itoa(mq2Val0, mq2Str0, 10);


	}
	else{		//groove
		ADMUX&=0b11111100;
		flag=0;

		grvVal = adcVal;

		itoa(grvVal, grvStr, 10);
	}
	ADCSRA |= (1<<ADSC);
}

ISR(INT2_vect){		//reset button
	_delay_ms(200);

	GIFR|= (1<<INT2);

	PORTB = 0 | (1<<valve) | (1<<powerLine) | (1<<2) ;


}

void fireDetected(){
	PORTB|= (1<<led);
	PORTB|= (1<<buzzer);
	PORTB|= (1<<pump);
	PORTB&= ~(1<<valve);
	PORTB&= ~(1<<powerLine);
}

void gasDetected(){
	PORTB|= (1<<led);
	PORTB|= (1<<buzzer);
	PORTB|= (1<<fan);
	PORTB&= ~(1<<valve);	
	PORTB&= ~(1<<powerLine);
}

void sendChar(unsigned char c){
    while (! (UCSRA & (1 << UDRE)) );
                        
    UDR = c;//once transmitter is ready sent eight bit data

    _delay_ms(20);
}

unsigned char recieveChar(){

	// Wait until RXC flag is set to logic 1
	while ((UCSRA & (1<<RXC)) == 0x00);
	return UDR; // Read the received character from UDR

}
int main(void)

{   

	//USART Init below:
    DDRD |= 1 << PIND1;//pin1 of portD as OUTPUT ?
    //DDRD &= ~(1 << PIND0);//pin0 of portD as INPUT ?
    PORTD |= 1 << PIND0;
    int UBBRValue = 12;//AS described before setting baud rate 9600BPS, double transmission
    UBRRH = (unsigned char) (UBBRValue >> 8);
    UBRRL = (unsigned char) UBBRValue;
    UCSRA = 2;
    UCSRB = (1 << RXEN) | (1 << TXEN);    //Enable the receiver and transmitter
    UCSRC = 0b10000110;

    DDRB = 0b11111011;			//?????
	PORTB = 0 | (1<<valve) | (1<<powerLine) | (1<<2); //?????

    DDRD = 0xFF;	//for LCD and ADC
	DDRC = 0xFF;


	//RESET Button Iterrupt Initialization below:
	MCUCSR|=1<<INT2;	//INT2->1->rising edge
	GICR|=(1<<INT2);	

	//ADC Init below:
	ADMUX = 0b01000000;		//for adc
	ADCSRA = 0b10001101;

	sei();

	ADCSRA |= (1<<ADSC);	//start ADC

	Lcd4_Init();

    while (1)

    {  
			Lcd4_Clear();
    		 Lcd4_Set_Cursor(1,0);
	 		 Lcd4_Write_String("Hello World");
			_delay_ms(30);

    	// unsigned char count;
    	// //count = 1;
    	// count = recieveChar();

    	// PORTA = count;
    	// PORTC = 1;
    	// _delay_ms(1000);

    }

    while(1)
	{	



	// 	if(mq2Val>mq2Threshold){
	// 		fireDetected();
	// 		Lcd4_Clear();
	// 		Lcd4_Set_Cursor(1,0);
	// 		Lcd4_Write_String("FIRE FIRE");


	// 		// str[0] = 0;
	// 		// strcpy(str, "Temp=");
	// 		// strcat(str, temparatureStr);
	// 		// strcat(str, "C");
	// 		// strcat(str, "           ");
	// 		// Lcd4_Set_Cursor(1,0);
	// 		// Lcd4_Write_String(str);

	// 		str[0] = 0;
	// 		strcpy(str, "Mq2=");
	// 		strcat(str, mq2Str);
	// 		strcat(str, " Grv=");
	// 		strcat(str, grvStr);
	// 		strcat(str, "           ");
	// 		Lcd4_Set_Cursor(2,0);
	// 		Lcd4_Write_String(str);


	// 	}
	// 	else if(grvVal>grvThreshold){
	// 		gasDetected();
	// 		Lcd4_Clear();
	// 		Lcd4_Set_Cursor(1,0);
	// 		Lcd4_Write_String("GAS GAS");
		
	// 		// str[0] = 0;
	// 		// strcpy(str, "Temp=");
	// 		// strcat(str, temparatureStr);
	// 		// strcat(str, "C");
	// 		// strcat(str, "           ");
	// 		// Lcd4_Set_Cursor(1,0);
	// 		// Lcd4_Write_String(str);

	// 		str[0] = 0;
	// 		strcpy(str, "Mq2=");
	// 		strcat(str, mq2Str);
	// 		strcat(str, " Grv=");
	// 		strcat(str, grvStr);
	// 		strcat(str, "           ");
	// 		Lcd4_Set_Cursor(2,0);
	// 		Lcd4_Write_String(str);




	// 	}

	// 	else{
	// 		str[0] = 0;
	// 		strcpy(str, "Temp=");
	// 		strcat(str, temparatureStr);
	// 		strcat(str, "C");
	// 		strcat(str, "           ");
	// 		Lcd4_Set_Cursor(1,0);
	// 		Lcd4_Write_String(str);

	// 		str[0] = 0;
	// 		strcpy(str, "Mq2=");
	// 		strcat(str, mq2Str);
	// 		strcat(str, " Grv=");
	// 		strcat(str, grvStr);
	// 		strcat(str, "           ");
	// 		Lcd4_Set_Cursor(2,0);
	// 		Lcd4_Write_String(str);

	// 	}


		
		
	// 	//Lcd4_Clear();
	// 	_delay_ms(30);
	 
	 }

}

 
