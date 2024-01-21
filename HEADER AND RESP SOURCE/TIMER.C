#include <LPC17xx.h>
#include <RTL.h>
#include <stdio.h>
#include "TIMER.H"
#define PRESCALE (3000-1)	 //for 3MHZ pclk our board

void Timer0_Init(void)
{
	LPC_SC->PCONP |= 0x0001;			 //set 0 th bir	to activate timer0
	LPC_SC->PCLKSEL0 = 0xFFF8;		   // to clear the [0:2] bits for timer
	LPC_TIM0->CTCR = 0x0000;		   //rising edge and 00 therefore in timer mode
	LPC_TIM0->PR = PRESCALE;			// put prescaler value in PR register
	LPC_TIM0->TCR = 0x02;				// RESET the timer once befor starting
}

void delayms(unsigned int milliseconds)
{
	LPC_TIM0->TCR = 0x02;	  // RESET the timer once befor starting
	LPC_TIM0->TCR = 0x01;	 //Start timer
	while(LPC_TIM0->TC < milliseconds);
	LPC_TIM0->TCR=0x00;
}
