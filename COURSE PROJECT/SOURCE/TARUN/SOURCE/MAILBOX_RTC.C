#include <RTL.h>                        // RTX kernel functions & defines      
#include <LPC17xx.h>
#include <stdio.h>
#include "UART.h"

#define CCR_CLKEN    0x00000001
#define CIIR_IMSEC   0x00000001
#define ILR_RTCCIF   0x00000001
#define CCR_CTCRST   0x00000002
#define ABORT        0x1B

OS_RESULT ret;
OS_TID tsk1, tsk2;  
os_mbx_declare(MsgBox, 1);             // Declare an RTX mailbox             
_declare_box(mpool, 30, 20); // Dynamic memory pool   

unsigned int rtc_read_data[3] = {0x3B, 0x3B, 0x0C}; // Initialize with valid values
volatile unsigned char RTC_Int;
char ascii_val[3][10];  // Changed to a 2D array to store strings

void UART0_Init(void);
void RTC_Init(void);
void RTC_Start(void);
void RTC_IRQHandler(void);
void UART_Print_RTC(void);
__task void init_task(void);
__task void send_task(void);
__task void rec_task(void);

int main() {
    SystemInit();
    SystemCoreClockUpdate();
    RTC_Init();
    UART0_Init();
    RTC_Start();
    _init_box(mpool, sizeof(mpool), sizeof(U32));   // initialize the 'mpool' memory for the membox dynamic allocation   
    os_sys_init_prio(init_task, 10); // Start the RTX Kernel
}

__task void init_task(void) {
    tsk1 = os_tsk_create(send_task, 1);  // send_task at priority 1 
    tsk2 = os_tsk_create(rec_task, 0);   // rec_task at priority 0
    os_tsk_delete_self();                // must delete itself before exiting
}

void RTC_Init(void) {
    LPC_SC->PCONP |= (1 << 9);
    LPC_RTC->AMR = 0;
    LPC_RTC->CIIR = 0;
    LPC_RTC->CCR = 0;

    NVIC_EnableIRQ(RTC_IRQn);
}

void RTC_Start(void) {
    LPC_RTC->CCR |= CCR_CLKEN;
    LPC_RTC->ILR |= ILR_RTCCIF;
    LPC_RTC->CIIR |= CIIR_IMSEC;
}

void RTC_IRQHandler(void) {
    unsigned int j = 0;
    volatile unsigned long *psfr;

    psfr = (volatile unsigned long *)&LPC_RTC->SEC;

    for (j = 0; j < 3; j++) { // Fixed the loop to read 3 values only
        rtc_read_data[j] = *psfr++;
    }

    RTC_Int = 0xff;

    LPC_RTC->ILR |= ILR_RTCCIF;
}

void UART_Print_RTC(void) {
    unsigned int j = 0;
    char *ptr1;

    while (RTC_Int == 0x00);

    for (j = 0; j < 8; j++) { // Changed the loop to match the data count
        sprintf(ascii_val[j], "%X", rtc_read_data[j]); // Using %X for hexadecimal conversion
    }

    for (j = 7; j >= 0; j--) { // Reversed the loop to print in correct order
        ptr1 = ascii_val[j];

        while (*ptr1 != '\0') {
            LPC_UART0->THR = *ptr1++;
            while ((LPC_UART0->LSR & 0x20) == 0);
        }

        LPC_UART0->THR = '\t';
        while ((LPC_UART0->LSR & 0x20) == 0);
    }

    LPC_UART0->THR = '\r';
    while ((LPC_UART0->LSR & 0x20) == 0);

    //RTC_Int = 0x00;
}

__task void rec_task(void) {
    U32 *rptr;

    while (1) {
        os_mbx_wait(MsgBox, (void *)&rptr, 0x02); // Wait for the message
        if (ret == OS_R_MBX) {
            os_dly_wait(2); // Adjust the delay as needed
        } else {
            // Handle other cases if necessary
        }
    }
}

__task void send_task(void) {
    U32 *mptr;
    os_mbx_declare(MsgBox, 1);
    os_mbx_init(MsgBox, sizeof(MsgBox));
    mptr = _alloc_box(mpool);

    while (1) {
        // Copy RTC data to the mailbox buffer
        mptr = rtc_read_data;
        // Send the message to the mailbox
        os_mbx_send(MsgBox, mptr, 0x02);
        UART_Print_RTC();
        os_dly_wait(2); // Adjust the delay as needed
    }
}


 void UART0_Init(void)
 {
	LPC_SC->PCONP |= 0x00000008;	//UART0 peripheral enable
	LPC_PINCON->PINSEL0 &= ~0x000000F0;
	LPC_PINCON->PINSEL0 |= 0x00000050;
	LPC_UART0->LCR = 0x00000083;	//enable divisor latch, parity disable, 1 stop bit, 8bit word length
	LPC_UART0->DLM = 0X00; 
	LPC_UART0->DLL = 0x13;      	//select baud rate 9600 bps
	LPC_UART0->LCR = 0X00000003;
	LPC_UART0->FCR = 0x07;
	LPC_UART0->IER = 0X03;	   		//select Transmit and receive interrupt

	NVIC_EnableIRQ(UART0_IRQn);		//Assigning channel
 } 
