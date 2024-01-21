#include <lpc17xx.h>
#include <RTL.h>
#include "BUZ.h"

void BUZZER_init()
{
	LPC_PINCON->PINSEL1 &= 0xFFFCFFFF;	//P0.24 is GPIO
	LPC_GPIO0->FIODIR |= 0x01000000;	//P0.24 is output
}

void BUZZER_On(int duration)
{
	os_itv_set (duration*100);     			// set wait interval:  1 second       
	for (;;) {
    os_itv_wait ();
    /* do some actions at regular time intervals */
	/* Buzzer ON for 1sec */
	LPC_GPIO0->FIOSET = 0x01000000;	//P0.24 made high to turn ON the buzzer
	}
}

void BUZZER_Off(int duration)
{
	os_itv_set (duration*100);     			// set wait interval:  1 second       
	for (;;) {
    os_itv_wait ();
    /* do some actions at regular time intervals */
	/* Buzzer ON for 1sec */
	LPC_GPIO0->FIOCLR = 0x01000000;	//P0.24 made high to turn ON the buzzer
	}
}
