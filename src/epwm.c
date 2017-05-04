/*
 * epwm.c
 *
 *  Created on: 27 kwi 2017
 *      Author: miki
 */

#include "epwm.h"

volatile unsigned int epwm_clkdiv_hs, epwm_clkdiv;
volatile unsigned int epwm_status;
volatile uint16_t epwm_period, epwm_compare;

void ePWM_init(void)
{
	epwm_status = 0;

	// GPIO for PWM
	// use EPWM4A (pwm4, channel A)
    GPIO_setPullUp(gpio_h, GPIO_Number_6, GPIO_PullUp_Disable);
    GPIO_setMode(gpio_h, GPIO_Number_6, GPIO_6_Mode_EPWM4A);

    // PWM clock
    CLK_disableTbClockSync(clk_h);
	CLK_enablePwmClock(clk_h, PWM_Number_4);

    // PWM div
	// there are different bits for high-speed clock div and normal clock div
    PWM_setHighSpeedClkDiv(pwm4_h, PWM_HspClkDiv_by_1);
    PWM_setClkDiv(pwm4_h, PWM_ClkDiv_by_1);

    epwm_clkdiv_hs = 1;
    epwm_clkdiv = 1;

    // Modes
	PWM_setSyncMode(pwm4_h, PWM_SyncMode_Disable);
	PWM_setRunMode(pwm4_h, PWM_RunMode_FreeRun);
    PWM_setCounterMode(pwm4_h, PWM_CounterMode_Up);
    PWM_disableCounterLoad(pwm4_h);
    PWM_setShadowMode_CmpA(pwm4_h, PWM_ShadowMode_Shadow);  // Load registers every ZERO
    PWM_setLoadMode_CmpA(pwm4_h, PWM_LoadMode_Zero);

    // Actions
    PWM_setActionQual_Zero_PwmA(pwm4_h, PWM_ActionQual_Set); // set PWM when cnt == 0
    PWM_setActionQual_CntUp_CmpA_PwmA(pwm4_h, PWM_ActionQual_Clear); // clear PWMA when cnt == cmpA
    //PWM_setActionQual_Period_PwmA(pwm4_h, PWM_ActionQual_Set); // set PWM when cnt == period
    //PWM_setActionQual_CntDown_CmpA_PwmA(pwm4_h, PWM_ActionQual_Clear); // clear PWM when counting down

    // PWM values
    PWM_setPeriod(pwm4_h, 0);
    PWM_setPhase(pwm4_h, 0);
    PWM_setCount(pwm4_h, 0);
    PWM_setCmpA(pwm4_h, 0);

    epwm_period = 0;
    epwm_compare = 0;

    // force PWM stop
    ePWM_stop();

    // Start sync clock
    CLK_enableTbClockSync(clk_h);
}

void ePWM_start(void)
{
    PWM_Obj *pwm4 = (PWM_Obj*)pwm4_h;

    epwm_status = 1;
    pwm4->AQCSFRC = (pwm4->AQCSFRC & (!0x03)) | 0; // Continuous Software Force on Output A - Forcing disabled
    PWM_setCounterMode(pwm4_h, PWM_CounterMode_Up);
}

void ePWM_stop(void)
{
    PWM_Obj *pwm4 = (PWM_Obj*)pwm4_h;

    pwm4->AQCSFRC = (pwm4->AQCSFRC & (!0x03)) | 0x01; // Continuous Software Force on Output A - Forces a continuous low on output A
    PWM_setCounterMode(pwm4_h, PWM_CounterMode_Stop);
    PWM_setCount(pwm4_h, 0);
    epwm_status = 0;
}

uint8_t ePWM_setClkDiv_hs(unsigned int div)
{
	uint8_t clkdiv_hs_allowed[8] = {1, 2, 4, 6, 8, 10, 12, 14};
	/*
		PWM_HspClkDiv_by_1=(0 << 7),
		PWM_HspClkDiv_by_2=(1 << 7),
		PWM_HspClkDiv_by_4=(2 << 7),
		PWM_HspClkDiv_by_6=(3 << 7),
		PWM_HspClkDiv_by_8=(4 << 7),
		PWM_HspClkDiv_by_10=(5 << 7),
		PWM_HspClkDiv_by_12=(6 << 7),
		PWM_HspClkDiv_by_14=(7 << 7)
    */

	uint8_t i, allowed = 0, reg_value = 0;
	unsigned int run = 0;

	// loop through allowed values table
	for (i = 0; i < 8; i++)
	{
		if (div == clkdiv_hs_allowed[i])
		{
			reg_value = (i << 7); // register value to set
			allowed = 1;
			break;
		}
	}

	if (!allowed) {
		return 0; // the argument is invalid
	}

	epwm_clkdiv_hs = div;

	// if PWM is running: stop it and save the status
	if (epwm_status) {
		ePWM_stop();
		run = 1;
	}

	// set the high speed clock div
	PWM_setHighSpeedClkDiv(pwm4_h, (PWM_HspClkDiv_e)reg_value);

	// if PWM was running before, start it
	if (run) {
		ePWM_start();
	}

	return 1;
}

uint8_t ePWM_setClkDiv(unsigned int div)
{
	uint8_t clkdiv_allowed[8] = {1, 2, 4, 8, 16, 32, 64, 128};
	/*
		PWM_ClkDiv_by_1=(0 << 10),
		PWM_ClkDiv_by_2=(1 << 10),
		PWM_ClkDiv_by_4=(2 << 10),
		PWM_ClkDiv_by_8=(3 << 10),
		PWM_ClkDiv_by_16=(4 << 10),
		PWM_ClkDiv_by_32=(5 << 10),
		PWM_ClkDiv_by_64=(6 << 10),
		PWM_ClkDiv_by_128=(7 << 10)
    */

	uint8_t i, allowed = 0, reg_value = 0;
	unsigned int run = 0;

	// loop through allowed values table
	for (i = 0; i < 8; i++)
	{
		if (div == clkdiv_allowed[i])
		{
			reg_value = (i << 10); // register value to set
			allowed = 1;
			break;
		}
	}

	if (!allowed) {
		return 0; // the argument is invalid
	}

	epwm_clkdiv = div;

	// if PWM is running: stop it and save the status
	if (epwm_status) {
		ePWM_stop();
		run = 1;
	}

	// set the high speed clock div
	PWM_setClkDiv(pwm4_h, (PWM_ClkDiv_e)reg_value);

	// if PWM was running before, start it
	if (run) {
		ePWM_start();
	}

	return 1;
}

uint8_t ePWM_setPeriod(uint16_t period)
{
	uint8_t run = 0;

	if (period < 2) {
		return 0; // the argument is invalid
	}

	// if PWM is running: stop it and save the status
	if (epwm_status) {
		ePWM_stop();
		run = 1;
	}

	// set the high speed clock div
	PWM_setPeriod(pwm4_h, period);
	epwm_period = period;

	// if PWM was running before, start it
	if (run) {
		ePWM_start();
	}

	return 1;
}

void ePWM_setCompare(uint16_t compare)
{
	PWM_setCmpA(pwm4_h, compare);
	epwm_compare = compare;
}

void ePWM_status_print(void)
{
	UART_printf("\r\n ### ePWM status ###\r\n");

	if (epwm_status) {
		UART_printf("PWM is running\r\n");
	} else {
		UART_printf("PWM is stopped\r\n");
	}

	UART_printf("\tclkdiv_hs = %u, clkdiv = %u\r\n", epwm_clkdiv_hs, epwm_clkdiv);
	UART_printf("\tPWM_clock = ");
	UART_print_unit((uint32_t)CPU_FREQ_HZ/((uint32_t)(epwm_clkdiv_hs*epwm_clkdiv)));
	UART_printf("Hz\r\n");

	if (epwm_period != 0)
	{
		UART_printf("\tPWM_period = %u\r\n", epwm_period);
		UART_printf("\tPWM_compare = %u\r\n", epwm_compare);

		UART_printf("\tPWM_frequency = ");
		UART_print_unit(((uint32_t)CPU_FREQ_HZ/((uint32_t)(epwm_clkdiv_hs*epwm_clkdiv)))/(uint32_t)epwm_period);
		UART_printf("Hz\r\n");

		UART_printf("\tPWM_duty = %u%%\r\n", (uint16_t)(((uint32_t)epwm_compare*100)/(uint32_t)epwm_period));
	}

	UART_printf(" ### -------- ###\r\n");
}
