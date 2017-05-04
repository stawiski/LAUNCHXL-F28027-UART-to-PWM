/*
 * system.h
 *
 *  Created on: 26 kwi 2017
 *      Author: miki
 */

#ifndef F28027_UART_TO_PWM_INC_SYSTEM_H_
#define F28027_UART_TO_PWM_INC_SYSTEM_H_

#include "global.h"
#include "uart.h" // include for peripherals ISR

extern void DSP28x_usDelay(Uint32 Count);
void setup_handles(void);
void init_system(void);

#endif /* F28027_UART_TO_PWM_INC_SYSTEM_H_ */
