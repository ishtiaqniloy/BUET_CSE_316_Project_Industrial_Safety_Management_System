
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
char valStr0[20];
char valStr1[20];

int flag = 0;

ISR(ADC_vect){
	uint16_t valLO = ADCL;
	uint16_t valHI = ADCH;

	
	uint16_t adcVal = valLO|(valHI<<8);
	//float stepSize=4.00/1024;
	//float analogVal=adcVal*stepSize;
	
	//ftoa(analogVal,valStr,2);

	if(flag==0){
		itoa(adcVal, valStr0, 10);
		ADMUX|=0b00000001;
		flag=1;

	}
	else{
		itoa(adcVal, valStr1, 10);
		ADMUX&=0b11111110;
		flag=0;

	}
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
	{	Lcd4_Clear();
		Lcd4_Set_Cursor(1,1);
		
		strcpy(str, "0:");
	
		strcat(str, valStr0);
		strcat(str, " 1:");
		strcat(str, valStr1);
		
		Lcd4_Write_String(str);
		
		
		_delay_ms(30);
	}

	}
