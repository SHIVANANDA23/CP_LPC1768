#include <RTL.h>                        // RTX kernel functions & defines      
#include <LPC17xx.h>                    // LPC17xx definitions                 
#include <stdio.h>
#include "UART.H"
#include "DC.H"
#include "BUZ.H"
#include "TIMER.H"

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

//BUZZER FUNCTION PROTOTYPE

extern void BUZZER_init(void);
extern void BUZZER_On(int duration);
extern void BUZZER_Off(int duration);

//LCD FUNCTION PROTOTYPES

extern void lcd_init(void);
extern void wr_cn(void);
extern void clr_disp(void);
extern void delay_lcd(int);
extern void lcd_com(void);						   
extern void wr_dn(void);
extern void lcd_data(void);
extern void clear_ports(void);
extern void lcd_puts(char *);

//TIMER0 

extern void Timer0_Init(void);
extern void delayms(unsigned int milliseconds);

void Ext_Int_init(void)
{
	LPC_PINCON->PINSEL4 = 0x04000000;
	LPC_SC->EXTINT = 0x00000008;		//writing 1 cleares the interrupt, get set if there is interrupt
	LPC_SC->EXTMODE = 0x00000008;		//EINT3 is initiated as edge senitive, 0 for level sensitive
	LPC_SC->EXTPOLAR = 0x00000000;		//EINT3 is falling edge sensitive, 1 for rising edge
	NVIC_EnableIRQ(EINT3_IRQn);
}

 void EINT3_IRQHandler(void)
 {
	 //Do some work
 	
	LPC_SC->EXTINT = 0x00000008;	//cleares the interrupt
 }

int main()
{
	Timer0_Init();
	Ext_Int_init();
	UART0_Init();
	DC_Init();
	BUZZER_init();
	UART0_SendString("Enter a number \n");
	x=UART0_RecieveChar();
	UART0_SendString("Entered number is : ");
	os_dly_wait(10);
	UART0_SendChar(x);
}
