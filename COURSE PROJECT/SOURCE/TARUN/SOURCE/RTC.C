#include<LPC17xx.h>
#include<stdio.h>
// Function to initialize RTC
void RTC_Init(void) {
	  
    LPC_RTC->HOUR = 12;
    LPC_RTC->MIN = 0;
    LPC_RTC->SEC = 0;
    LPC_SC->PCONP |= (1 << 9); // Enable power to RTC

    LPC_RTC->CCR = 0x01; // Enable clock for RTC

    // RTC initialization settings
    LPC_RTC->AMR = 0x00; // Disable alarm
    LPC_RTC->CIIR = 0x00; // Disable interrupt


    // Start RTC
    LPC_RTC->CCR |= 0x02;
}

