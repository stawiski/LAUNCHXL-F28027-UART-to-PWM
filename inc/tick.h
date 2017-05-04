/*
 * tick.h
 *
 *  Created on: 26 kwi 2017
 *      Author: miki
 */

#ifndef F28027_UART_TO_PWM_INC_TICK_H_
#define F28027_UART_TO_PWM_INC_TICK_H_

#include "global.h"

extern TIMER_Handle tick_timer;
extern volatile uint32_t tick_counter;

void tick_init(void);
interrupt void tick_isr(void);

#endif /* F28027_UART_TO_PWM_INC_TICK_H_ */
