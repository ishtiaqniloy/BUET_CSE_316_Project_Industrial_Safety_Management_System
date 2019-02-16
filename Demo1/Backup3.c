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

//#include <stdio.h>
#include<avr/io.h>
#include<avr/interrupt.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>
#include "lcd.h"
#include "ftoa.h"
//using namespace std;

char str[100];
int flag = 0;

float temparature = 0;	//adc0
char temparatureStr[20];

int mq2Val = 0;			//adc1
char mq2Str[20];

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

		temparature = adcVal*TempStepSize;
		ftoa(temparature,temparatureStr,2);
	}
	else if(flag==1){	//mq2
		ADMUX&=0b11111110;
		ADMUX|=0b00000010;
		flag=2;

		mq2Val = adcVal;
		itoa(mq2Val, mq2Str, 10);


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


int main(void)
{

	// str[0]=0;
	// mq2Str[0]='0';
	// mq2Str[1]=0;
	// grooveStr[0]='0';
	// grooveStr[1]=0;

	DDRB = 0b11111011;			//?????
	PORTB = 0 | (1<<valve) | (1<<powerLine) | (1<<2); //?????

    DDRD = 0xFF;	//for LCD and ADC
	DDRC = 0xFF;
	
	MCUCSR|=1<<INT2;	//INT2->1->rising edge

	GICR|=(1<<INT2);	


	ADMUX = 0b01000000;		//for adc
	ADCSRA = 0b10001101;

	sei();

	ADCSRA |= (1<<ADSC);

	Lcd4_Init();

	while(1)
	{	

		if(mq2Val>mq2Threshold){
			fireDetected();
			Lcd4_Clear();
			Lcd4_Set_Cursor(1,0);
			Lcd4_Write_String("FIRE FIRE");


			// str[0] = 0;
			// strcpy(str, "Temp=");
			// strcat(str, temparatureStr);
			// strcat(str, "C");
			// strcat(str, "           ");
			// Lcd4_Set_Cursor(1,0);
			// Lcd4_Write_String(str);

			str[0] = 0;
			strcpy(str, "Mq2=");
			strcat(str, mq2Str);
			strcat(str, " Grv=");
			strcat(str, grvStr);
			strcat(str, "           ");
			Lcd4_Set_Cursor(2,0);
			Lcd4_Write_String(str);


		}
		else if(grvVal>grvThreshold){
			gasDetected();
			Lcd4_Clear();
			Lcd4_Set_Cursor(1,0);
			Lcd4_Write_String("GAS GAS");
		
			// str[0] = 0;
			// strcpy(str, "Temp=");
			// strcat(str, temparatureStr);
			// strcat(str, "C");
			// strcat(str, "           ");
			// Lcd4_Set_Cursor(1,0);
			// Lcd4_Write_String(str);

			str[0] = 0;
			strcpy(str, "Mq2=");
			strcat(str, mq2Str);
			strcat(str, " Grv=");
			strcat(str, grvStr);
			strcat(str, "           ");
			Lcd4_Set_Cursor(2,0);
			Lcd4_Write_String(str);




		}

		else{
			str[0] = 0;
			strcpy(str, "Temp=");
			strcat(str, temparatureStr);
			strcat(str, "C");
			strcat(str, "           ");
			Lcd4_Set_Cursor(1,0);
			Lcd4_Write_String(str);

			str[0] = 0;
			strcpy(str, "Mq2=");
			strcat(str, mq2Str);
			strcat(str, " Grv=");
			strcat(str, grvStr);
			strcat(str, "           ");
			Lcd4_Set_Cursor(2,0);
			Lcd4_Write_String(str);

		}


		
		
		//Lcd4_Clear();
		_delay_ms(30);
	}

	}
