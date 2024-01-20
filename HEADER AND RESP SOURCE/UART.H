#ifndef UART_H
#define UART_H

void UART0_Init(void) ;
char UART0_RecieveChar(void);
void UART0_SendChar(char a);
void UART0_SendString(char *ptr);

#endif
