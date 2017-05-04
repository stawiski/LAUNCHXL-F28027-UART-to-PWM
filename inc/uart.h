/*
 * uart.h
 *
 *  Created on: 26 kwi 2017
 *      Author: miki
 */

#ifndef F28027_UART_TO_PWM_INC_UART_H_
#define F28027_UART_TO_PWM_INC_UART_H_

#include "global.h"

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
PINS:
- GPIO28 as SCI_RX
- GPIO29 as SCI_TX
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

struct UART_data_chunk_s
{
	uint8_t index;
	uint16_t length; // max 1024
	uint8_t data[128];
};

//extern struct UART_data_chunk_s g_UART_data_chunk;

void UART_init(void);
interrupt void UART_TXRDY_ISR(void);

void UART_putc(unsigned char byte);
void UART_puts(char *s);
void UART_sendByte(unsigned char byte);
void UART_printf(char *format, ...);

unsigned char UART_readByte(void); // read byte (if available); returns 0 at no success; returns the received character at success;
unsigned char UART_readByteWait(void); // call readByte() until character is received (blocks the thread)
uint16_t UART_readUint16Wait(bool echo_back); // read numerical characters combining them into a number of uint16_t type; "enter" finishes the typing; argument echo_back enables writing processed numerical characters back to the terminal (so that the user knows what has been typed);
void UART_print_unit(uint32_t value); // print value of a unit, formatting it to kilo/Mega if neccessary

#endif /* F28027_UART_TO_PWM_INC_UART_H_ */
