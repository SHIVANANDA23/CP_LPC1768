//MAILBOX BASIC CODE

/* this program demonstrate the usage of mailbox , task1 will count 0to15 and send 15 to task2 via mailbox
task2 will increment count value from 15 to 30 and then task1 resumes*/

#include <rtl.h>
#include<stdio.h>
#include<lpc214x.h>
void init_serial(void);
os_mbx_declare (MsgBox, 100);                /* Declare an RTX mailbox  100 msgs with name MsgBox*/
       
_declare_box(mpool,20,32);	 /* Reserve a memory for 32 blocks of 20 bytes  */
unsigned int cnt1,cnt2;
char arr1[20],arr2[20];
int i=0;
	
__task void task2 (void);

__task void task1 (void)
 {
  /* This task will send a count value. */
  U32 *mptr;
  os_tsk_create (task2, 0);
  os_mbx_init (MsgBox, sizeof(MsgBox));
  mptr = _alloc_box (mpool);                /* Allocate a memory for the message */
 while(1)
 {
 	 while(!(cnt1==15))
 			{ 
			cnt1++; 
 
 			sprintf(arr1,"counter1 :%d",cnt1);
  			while (arr1[i] != '\0')
			{	os_dly_wait(1);
	  			while (!(U0LSR & 0x20));
	  			U0THR = arr1[i];
				i++;
			}
			i=0;
			while (!(U0LSR & 0x20));
			U0THR = '\n'; 
			 os_dly_wait(5);
	 		}
 
   	 if (cnt1==15)
		{
  		mptr[0] = cnt1; 
		cnt1=0;
   		os_mbx_send (MsgBox, mptr, 0xffff);	 /* Send the count value to a 'Mailbox' */
		os_dly_wait(5);
     
	 	}
   	 }
    
   }
  
__task void task2 (void) 
{
  /* This task will receive a count. */
  U32 *rptr  ;
  cnt2=0;
 	 while(1)
	 { 
	 os_mbx_wait (MsgBox, (void*)&rptr, 0xffff);      / Wait for the message to arrive. */
     cnt2 = rptr[0];	    /* copy the count value from task1 to cnt2  */

	while(!(cnt2==30))
	{	 
			cnt2++;
		 	sprintf(arr2,"counter2 :%d",cnt2);
		os_dly_wait(2);
  			while (arr2[i] != '\0')
			{
	  		while (!(U0LSR & 0x20));
	  		U0THR = arr2[i];
			i++;
			}
			i=0;
			while (!(U0LSR & 0x20));
			U0THR = '\n'; 
			os_dly_wait(5);
	 	  }
		  
	 	cnt2=0;
	  }
 	 }


int main (void) 
{
 init_serial();
  _init_box (mpool, sizeof(mpool), sizeof(U32));
  os_sys_init(task1);
}

/*----------------------------------------------------------------------------
 *       init_serial:  Initialize Serial Interface
 *---------------------------------------------------------------------------*/
void init_serial (void) 
{
  PINSEL0 = 0X0000005;                 // Enable RxD0 and TxD0              
  U0LCR = 0x83;                         // 8 bits, no Parity, 1 Stop bit     
  U0DLL = 0x61;                           // 9600 Baud Rate @ 15MHz VPB Clock  
  U0LCR = 0x03;                         // DLAB = 0                          
}