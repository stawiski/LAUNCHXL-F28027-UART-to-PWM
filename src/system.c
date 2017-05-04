/*
 * system.c
 *
 *  Created on: 26 kwi 2017
 *      Author: miki
 */

#include "system.h"

void setup_handles(void)
{
	clk_h = CLK_init((void *)CLK_BASE_ADDR, sizeof(CLK_Obj));
	pll_h = PLL_init((void *)PLL_BASE_ADDR, sizeof(PLL_Obj));
	wdog_h = WDOG_init((void *)WDOG_BASE_ADDR, sizeof(WDOG_Obj));
	cpu_h = CPU_init((void *)NULL, sizeof(CPU_Obj));

	flash_h = FLASH_init((void *)FLASH_BASE_ADDR, sizeof(FLASH_Obj));
	gpio_h = GPIO_init((void *)GPIO_BASE_ADDR, sizeof(GPIO_Obj));
	pie_h = PIE_init((void *)PIE_BASE_ADDR, sizeof(PIE_Obj));
	adc_h = ADC_init((void *)ADC_BASE_ADDR, sizeof(ADC_Obj));
	sci_h = SCI_init((void *)SCIA_BASE_ADDR, sizeof(SCI_Obj));

	pwm1_h = PWM_init((void *)PWM_ePWM1_BASE_ADDR, sizeof(PWM_Obj));
	pwm2_h = PWM_init((void *)PWM_ePWM2_BASE_ADDR, sizeof(PWM_Obj));
	pwm3_h = PWM_init((void *)PWM_ePWM3_BASE_ADDR, sizeof(PWM_Obj));
	pwm4_h = PWM_init((void *)PWM_ePWM4_BASE_ADDR, sizeof(PWM_Obj));

	clk = (CLK_Obj *)clk_h;
	gpio = (GPIO_Obj *)gpio_h;
	sci = (SCI_Obj *)sci_h;
}

void init_system(void)
{
	WDOG_disable(wdog_h);
	(*Device_cal)(); // calibrate ADC & internal oscillators
	CLK_setOscSrc(clk_h, CLK_OscSrc_Internal); // run the MCU from internal oscillator
	PLL_setup(pll_h, PLL_Multiplier_12, PLL_DivideSelect_ClkIn_by_2); // internal oscillator runs at 10MHz (SYSCLKOUT = LSPCLK = 10MHz * 12 / 2 = 60MHz)

	// Set the Low-Speed Peripheral Clock Prescaler. Default value: SYSCLKOUT/4. If LOSPCP = 0, then LSPCLK = SYSCLKOUT.

	ENABLE_PROTECTED_REGISTER_WRITE_MODE;
	clk->LOSPCP = 0;
	DISABLE_PROTECTED_REGISTER_WRITE_MODE;

	uint16_t register i;
	for (i = 0; i < 0xFFFF; i++) {} // wait for clocks to settle

	PIE_disable(pie_h);
	PIE_disableAllInts(pie_h);
	CPU_disableGlobalInts(cpu_h);
	CPU_clearIntFlags(cpu_h);

	//PIE_setDebugIntVectorTable(pie_h);

	#ifdef _FLASH
		memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (size_t)&RamfuncsLoadSize);
		FLASH_setup(flash_h);
	#endif
}
