#define _USART_H_


void putUsart0(unsigned char c);

void usartInit(void);

#define WS_UCSRA UCSRA
#define WS_UDR UDR






static int uart_putchar(char c, FILE *stream);  //Add custom putchar function  

//static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL,_FDEV_SETUP_WRITE); 


//From RS232 sends a byte
static int uart_putchar(char c, FILE *stream)    
{ 
  if (c == '\n')        
  uart_putchar('\r', stream);      
  loop_until_bit_is_set(UCSRA, UDRE);      
  UDR = c;      
  return 0;    
} 

void putUsart0(unsigned char c)
{ 
	while(!(WS_UCSRA & 0x20));
	WS_UDR = c;
}
//From receiving a RS232 bytes
unsigned int getUsart0(void)
{
    unsigned char status,res;
	if(!(WS_UCSRA & 0x80)) return 0xFFFF;        //no data to be received 
    status = WS_UCSRA;
    res = WS_UDR;
    if (status & 0x1c) return 0xFFFF;        // If error, return 0xFFFF 
    return res;
}
//Waiting for receiving a from RS232 effective bytes
unsigned char waitUsart0(void)
{
    unsigned int c;
    while((c=getUsart0())==0xFFFF);
    return (unsigned char)c;
}



void usartInit(void)
{
	// USART0 initialization
	// Communication Parameters: 8 Data, 1 Stop, No Parity
	// USART0 Receiver: On
	// USART0 Transmitter: On
	// USART0 Mode: Asynchronous
	// USART0 Baud Rate: 9600
	//
	UCSRA=0x00;
	UCSRB=0x98;
	UCSRC=0x06;
	UBRRH=0x00;
	UBRRL=0x2F;
	//stdout = &mystdout;       //Must add 
}


