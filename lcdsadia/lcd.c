// ---------------------------------------------------------------------------
// HD44780-LCD DRIVER ROUTINES
//
// Routines:
// LCD_Init initializes the LCD controller
// LCD_Cmd sends LCD controller command
// LCD_Char sends single ascii character to display
// LCD_Clear clears the LCD display & homes cursor
// LCD_Home homes the LCD cursor
// LCD_Goto puts cursor at position (x,y)
// LCD_Line puts cursor at start of line (x)
// LCD_Hex displays a hexadecimal value
// LCD_Integer displays an integer value
// LCD_String displays a string
//
// The LCD module requires 6 I/O pins: 2 control lines & 4 data lines.
// PortB is used for data communications with the HD44780-controlled LCD.
// The following defines specify which port pins connect to the controller:
#define LCD_RS 0 // pin for LCD R/S (eg PB0)
#define LCD_E 1 // pin for LCD enable
#define DAT4 2 // pin for d4
#define DAT5 3 // pin for d5
#define DAT6 4 // pin for d6
#define DAT7 5 // pin for d7
#define ClearBit(x,y) x &= ~_BV(y) // equivalent to cbi(x,y)
#define SetBit(x,y) x |= _BV(y) // equivalent to sbi(x,y)
// The following defines are HD44780 controller commands
#define CLEARDISPLAY 0x01
#define SETCURSOR 0x80

typedef uint8_t byte; // I just like byte & sbyte better
typedef int8_t sbyte;

void msDelay(int delay) // put into a routine
{ // to remove code inlining
int i;
for ( i=0;i<delay;i++) // at cost of timing accuracy
_delay_ms(1);
}


void PulseEnableLine ()
{
SetBit(PORTB,LCD_E); // take LCD enable line high
_delay_us(40); // wait 40 microseconds
ClearBit(PORTB,LCD_E); // take LCD enable line low
}



void SendNibble(byte data)
{
PORTB &= 0xC3; // 1100.0011 = clear 4 data lines
if (data & _BV(4)) SetBit(PORTB,DAT4);
if (data & _BV(5)) SetBit(PORTB,DAT5);
if (data & _BV(6)) SetBit(PORTB,DAT6);
if (data & _BV(7)) SetBit(PORTB,DAT7);
PulseEnableLine(); // clock 4 bits into controller
}



void SendByte (byte data)
{
SendNibble(data); // send upper 4 bits
SendNibble(data<<4); // send lower 4 bits
ClearBit(PORTB,5); // turn off boarduino LED
}



void LCD_Cmd (byte cmd)
{
ClearBit(PORTB,LCD_RS); // R/S line 0 = command data
SendByte(cmd); // send it
}



void LCD_Char (byte ch)
{
SetBit(PORTB,LCD_RS); // R/S line 1 = character data
SendByte(ch); // send it
}



void LCD_Init()
{
LCD_Cmd(0x33); // initialize controller
LCD_Cmd(0x32); // set to 4-bit input mode
LCD_Cmd(0x28); // 2 line, 5x7 matrix
LCD_Cmd(0x0C); // turn cursor off (0x0E to enable)
LCD_Cmd(0x06); // cursor direction = right
LCD_Cmd(0x01); // start with clear display
msDelay(3); // wait for LCD to initialize
}


void LCD_Clear() // clear the LCD display
{
LCD_Cmd(CLEARDISPLAY);
msDelay(3); // wait for LCD to process command
}


void LCD_Home() // home LCD cursor (without clearing)
{
LCD_Cmd(SETCURSOR);
}


void LCD_Goto(byte x, byte y) // put LCD cursor on specified line
{
byte addr = 0; // line 0 begins at addr 0x00
switch (y)
{
case 1: addr = 0x40; break; // line 1 begins at addr 0x40
case 2: addr = 0x14; break;
case 3: addr = 0x54; break;
}

LCD_Cmd(SETCURSOR+addr+x); // update cursor with x,y position
}



void LCD_Line(byte row) // put cursor on specified line
{
LCD_Goto(0,row);
}



void LCD_String(const char *text) // display string on LCD
{
while (*text) // do until /0 character
LCD_Char(*text++); // send char & update char pointer
}




void LCD_Hex(int data)
// displays the hex value of DATA at current LCD cursor position
{
char st[8] = ""; // save enough space for result
itoa(data,st,16); // convert to ascii hex
//LCD_Message("0x"); // add prefix "0x" if desired
LCD_String(st); // display it on LCD
}



void LCD_Integer(int data)
// displays the integer value of DATA at current LCD cursor position
{
char st[8] = ""; // save enough space for result
itoa(data,st,10); // convert to ascii
LCD_String(st); // display in on LCD
}
