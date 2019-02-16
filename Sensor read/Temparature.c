
#ifndef F_CPU
#define F_CPU 1000000 // 1 MHz clock speed
#endif

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
char str[50];
char valStr[20];

ISR(ADC_vect){
	uint16_t valLO = ADCL;
	uint16_t valHI = ADCH;

	
	uint16_t adcVal = valLO|(valHI<<8);
	float stepSize=(4.57*1000)/(1024*10);	//for first way of temparature
	float analogVal=adcVal*stepSize;
	
	ftoa(analogVal,valStr,2);

	//itoa(adcVal, valStr, 10);

	ADCSRA |= (1<<ADSC);
}


int main(void)
{
    DDRD = 0xFF;
	DDRC = 0xFF;
	//int i;
	Lcd4_Init();
	


	ADMUX = 0b01000000;
	ADCSRA = 0b10001101;

	sei();

	ADCSRA |= (1<<ADSC);
	
	while(1)
	{	//Lcd4_Clear();
		Lcd4_Set_Cursor(1,1);
		
		strcpy(str, "Value = ");
	
		strcat(str, valStr);
		//strcat(str, " V");

		Lcd4_Write_String(str);
		
		
		_delay_ms(30);
	}

	}
