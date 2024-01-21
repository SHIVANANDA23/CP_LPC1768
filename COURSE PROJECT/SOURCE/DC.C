#include <LPC17xx.h>
#include <RTL.h>
#include <stdio.h>
#include "DC.H"

void CLOCK_WISE()
{
	LPC_GPIO0->FIOCLR = 0x00800000;	// P0.23 Kept low to off DCM
  os_dly_wait(1);				 	// delay to componsate inertia
  LPC_GPIO0->FIOSET = 0x04800000;	// Both coil and motor is on
}

void ANTICLOCK_WISE()
{
	LPC_GPIO0->FIOCLR = 0x00800000;	// P0.23 Kept low to off DCM
	os_dly_wait(1);									// delay to componsate inertia
	LPC_GPIO0->FIOCLR = 0x04000000;	// coil is off
	LPC_GPIO0->FIOSET = 0x00800000;	// Motor is on
}

void DC_Init()
{
	LPC_PINCON->PINSEL1 &= 0xFFCF3FFF;			// P0.23, P0.26 GPIO. P0.23 for on & off, P0.26 controls dir
	LPC_GPIO0->FIODIR |= 0x04800000;			// P0.23 and P0.26 output
}
