#include <RTL.h>                        // RTX kernel functions & defines      
#include <LPC17xx.h>                    // LPC17xx definitions                 
#include <stdio.h>
#include "UART.H"
#include "DC.H"

int i;
char x='a',x1[29];


// UART FUNCTION PROTOTYPES

extern void UART0_Init(void);
extern void UART0_SendString(char *ptr);
extern void UART0_SendChar(char a);
extern char UART0_RecieveChar(void);

//DC MOTOR FUNCTION PROTYPES

extern void DC_Init(void);
extern void ANTICLOCK_WISE(void);
extern void CLOCK_WISE(void);

int main()
{
	UART0_Init();
	DC_Init();
	ANTICLOCK_WISE();
	LPC_GPIO0->FIODIR=0x00;
	while(1){
	if((LPC_GPIO0->FIOPIN&0x10)==0x10)
		UART0_SendString("IR\n");
	else
		UART0_SendString("I AM\n");}
	for(;i<10;i++)
	{
		os_dly_wait(1000);
		sprintf(x1,"Digree : %d \n",i*36);
		UART0_SendString(x1);
	}
	CLOCK_WISE();
	
	UART0_SendString("Enter a number \n");
	x=UART0_RecieveChar();
	UART0_SendString("Entered number is : ");
	os_dly_wait(10);
	UART0_SendChar(x);
}
