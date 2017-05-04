/*
 * leds.h
 *
 *  Created on: 26 kwi 2017
 *      Author: miki
 */

#ifndef F28027_UART_TO_PWM_INC_LEDS_H_
#define F28027_UART_TO_PWM_INC_LEDS_H_

#include "global.h"

// LEDs available

#define LED_TICK	0 // LED for tick counter
#define LED_1		1
#define LED_2		2
#define LED_3		3

void leds_init(void);
void led_on(unsigned int led_number);
void led_off(unsigned int led_number);
void led_toggle(unsigned int led_number);

#endif /* F28027_UART_TO_PWM_INC_LEDS_H_ */
