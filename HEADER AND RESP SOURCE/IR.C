#include <lpc17xx.h>
#include <RTL.h>
#include "IR.h"

void IR_init(void)
{
	LPC_PINCON->PINSEL1 &= 0xFFFF3FFF;	//P0.23 is GPIO
	LPC_GPIO0->FIODIR |= 0x00000000;	//P0.23 is output
}

int is_detected(void)
{
	if((LPC_GPIO0->FIOPIN0&0x00800000)==0x00800000)
		return 1;
	else
		return 0;
}
