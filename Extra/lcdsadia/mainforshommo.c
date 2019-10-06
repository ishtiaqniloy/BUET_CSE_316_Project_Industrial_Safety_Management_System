#define F_CPU 16000000L // run CPU at 16 MHz
#define LED 5 // Boarduino LED on PB5
#define ClearBit(x,y) x &= ~_BV(y) // equivalent to cbi(x,y)
#define SetBit(x,y) x |= _BV(y) // equivalent to sbi(x,y)
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include "lcd.c"

typedef uint8_t byte;
typedef int8_t sbyte;


void InitAVR()
{
DDRB = 0x3F;
DDRC = 0x00;
DDRD=0xFF;
}

void FlashLED()
{
SetBit(PORTB,LED);
msDelay(250);
ClearBit(PORTB,LED);
msDelay(250);
}

void LCD_TwoDigits(byte data)
// helper function for WriteDate()
// input is two digits in BCD format
// output is to LCD display at current cursor position
{
//char c;
byte temp = data>>4;
LCD_Char(temp+'0');
//c=temp+'0';
data &= 0x0F;
LCD_Char(data+'0');
//c+=data+'0';
//int k=(int)c;
}
int main(void)
{


InitAVR(); // set port direction
LCD_Init();
LCD_String("Ready.");


}
