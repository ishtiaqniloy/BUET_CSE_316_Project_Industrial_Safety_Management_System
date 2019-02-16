
#ifndef F_CPU
#define F_CPU 16000000UL // 16 MHz clock speed
#endif


#define D4 eS_PORTD4
#define D5 eS_PORTD5
#define D6 eS_PORTD6
#define D7 eS_PORTD7
#define RS eS_PORTC6
#define EN eS_PORTC7



#include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>
#include<stdio.h>
#include"NRF24L01.h"
#include"USART.h"
#include "lcd.h"

#define SEND 1	//1->send 0->receive


extern uint8_t RX_BUF[];
extern uint8_t TX_BUF[];

int main(void)
{	
	// DDRD = 0xFF;
 //   	DDRC = 0xFF;
 //   	int i;
 //   	Lcd4_Init();


	DDRA = 0xFF;
	PORTA = 0xFF;

	usartInit();
	// printf("*--------------------------------------------------- *\r\n");
	// printf("*-------------------NRF24L01 experiment-------------- *\r\n");
	// printf("*--------------------------------------------------- *\r\n");

	nRF24L01_Initial();
	

	
		if(SEND){
			PORTA = 0;
			TX_BUF[0]=0x55;
			NRF24L01_Send();
			PORTA = 1;
			//printf("\r\nSend data£º0x%x\r\n",TX_BUF[0]);
			_delay_ms(1000);
		}
		else{
			NRF24L01_Receive();
			if(RX_BUF[0]==0x55)
				PORTA = 0xFF;
		}	

	

	while(1)
	{	
		// Lcd4_Set_Cursor(1,0);
		// Lcd4_Write_String("electroSome LCD Hello World");
		


		// Lcd4_Clear();		
		// _delay_ms(2000);


	}

}




			
