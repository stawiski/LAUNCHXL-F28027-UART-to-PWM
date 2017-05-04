/*
 * pwm.h
 *
 *  Created on: 27 kwi 2017
 *      Author: miki
 */

#ifndef F28027_UART_TO_PWM_INC_EPWM_H_
#define F28027_UART_TO_PWM_INC_EPWM_H_

#include "global.h"

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
PINS:
- GPIO6 as EPWM4A
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

// PWM module clock = SYSCLKOUT / (HSPCLKDIV × CLKDIV)
extern volatile unsigned int 		epwm_clkdiv_hs, // High-speed clock div
									epwm_clkdiv; // time base clock div

extern volatile unsigned int		epwm_status;
extern volatile uint16_t			epwm_period, epwm_compare;

void ePWM_init(void);
void ePWM_start(void);
void ePWM_stop(void);

uint8_t ePWM_setClkDiv_hs(unsigned int div);
uint8_t ePWM_setClkDiv(unsigned int div);
uint8_t ePWM_setPeriod(uint16_t period);
void ePWM_setCompare(uint16_t compare);
void ePWM_status_print(void);

#endif /* INC_PWM_H_ */
