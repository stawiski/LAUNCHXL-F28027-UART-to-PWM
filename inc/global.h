/*
 * global.h
 *
 *  Created on: 26 kwi 2017
 *      Author: miki
 */

#ifndef F28027_UART_TO_PWM_INC_GLOBAL_H_
#define F28027_UART_TO_PWM_INC_GLOBAL_H_

#define CPU_FREQ_MHZ	60
#define CPU_FREQ_HZ		(CPU_FREQ_MHZ*1000000)

#include <stdio.h>
#include <file.h>

#include "F2802x_Device.h"
#include "f2802x_common/include/F2802x_Examples.h"

#include "f2802x_common/include/adc.h"
#include "f2802x_common/include/clk.h"
#include "f2802x_common/include/cpu.h"
#include "f2802x_common/include/flash.h"
#include "f2802x_common/include/gpio.h"
#include "f2802x_common/include/pie.h"
#include "f2802x_common/include/pll.h"
#include "f2802x_common/include/pwm.h"
#include "f2802x_common/include/sci_io.h"
#include "f2802x_common/include/sci.h"
#include "f2802x_common/include/timer.h"
#include "f2802x_common/include/wdog.h"

#include "system.h"
#include "tick.h"
#include "uart.h"
#include "leds.h"
#include "epwm.h"

extern CPU_Handle cpu_h;
extern PLL_Handle pll_h;
extern WDOG_Handle wdog_h;
extern CLK_Handle clk_h;

extern FLASH_Handle flash_h;
extern GPIO_Handle gpio_h;
extern PIE_Handle pie_h;
extern ADC_Handle adc_h;
extern SCI_Handle sci_h;

extern PWM_Handle pwm1_h;
extern PWM_Handle pwm2_h;
extern PWM_Handle pwm3_h;
extern PWM_Handle pwm4_h;

extern CLK_Obj *clk;
extern GPIO_Obj *gpio;
extern SCI_Obj *sci;

void kick_watchdog(void); // empty function as watchdog is not used

#endif /* F28027_UART_TO_PWM_INC_GLOBAL_H_ */
