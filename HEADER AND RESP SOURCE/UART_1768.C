#include <LPC17xx.h>
#include <RTL.h>
#include <stdio.h>
#include "UART.H"
char x;
void UART0_SendString(char *ptr)
{
	while(*ptr!='\0')
	{
		UART0_SendChar(*ptr);
		ptr++;
	}
}

void UART0_SendChar(char a)
{
	while(!(LPC_UART0->LSR&0x20));
	LPC_UART0->THR=a;
}

void UART0_RecieveChar()
{
	while(!(LPC_UART0->LSR&0x20));
	x=LPC_UART0->RBR;
}


void UART0_Init(void) 
{
	LPC_SC->PCONP |= 0x00000008;			// UART0 peripheral enable
	LPC_PINCON->PINSEL0 &= ~0x000000F0;
	LPC_PINCON->PINSEL0 |= 0x00000050;		// P0.2 - TXD0 and P0.3 - RXD0
	LPC_UART0->LCR = 0x00000083;			// enable divisor latch, parity disable, 1 stop bit, 8bit word length
	LPC_UART0->DLM = 0X00; 
	LPC_UART0->DLL = 0xA2;      			// select baud rate 9600 bps @25MHz
	LPC_UART0->LCR = 0X00000003;
	LPC_UART0->FCR = 0x07;
	LPC_UART0->IER = 0X03;	   				// select Transmit and receive interrupt
}
