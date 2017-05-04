/*
 * global.c
 *
 *  Created on: 26 kwi 2017
 *      Author: miki
 */

#include "global.h"

CPU_Handle cpu_h;
PLL_Handle pll_h;
WDOG_Handle wdog_h;
CLK_Handle clk_h;

FLASH_Handle flash_h;
GPIO_Handle gpio_h;
PIE_Handle pie_h;
ADC_Handle adc_h;
SCI_Handle sci_h;

PWM_Handle pwm1_h;
PWM_Handle pwm2_h;
PWM_Handle pwm3_h;
PWM_Handle pwm4_h;

CLK_Obj *clk;
GPIO_Obj *gpio;
SCI_Obj *sci;

void kick_watchdog(void)
{
}
