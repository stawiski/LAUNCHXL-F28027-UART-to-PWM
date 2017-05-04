/*
 * leds.c
 *
 *  Created on: 26 kwi 2017
 *      Author: miki
 */

#include "leds.h"

void leds_init(void)
{
	GPIO_setPullUp(gpio_h, GPIO_Number_0, GPIO_PullUp_Disable);
	GPIO_setPullUp(gpio_h, GPIO_Number_1, GPIO_PullUp_Disable);
	GPIO_setPullUp(gpio_h, GPIO_Number_2, GPIO_PullUp_Disable);
	GPIO_setPullUp(gpio_h, GPIO_Number_3, GPIO_PullUp_Disable);

	GPIO_setMode(gpio_h, GPIO_Number_0, GPIO_0_Mode_GeneralPurpose);
	GPIO_setMode(gpio_h, GPIO_Number_1, GPIO_1_Mode_GeneralPurpose);
	GPIO_setMode(gpio_h, GPIO_Number_2, GPIO_2_Mode_GeneralPurpose);
	GPIO_setMode(gpio_h, GPIO_Number_3, GPIO_3_Mode_GeneralPurpose);

	GPIO_setDirection(gpio_h, GPIO_Number_0, GPIO_Direction_Output);
	GPIO_setDirection(gpio_h, GPIO_Number_1, GPIO_Direction_Output);
	GPIO_setDirection(gpio_h, GPIO_Number_2, GPIO_Direction_Output);
	GPIO_setDirection(gpio_h, GPIO_Number_3, GPIO_Direction_Output);

	GPIO_setHigh(gpio_h, GPIO_Number_0);
	GPIO_setHigh(gpio_h, GPIO_Number_1);
	GPIO_setHigh(gpio_h, GPIO_Number_2);
	GPIO_setHigh(gpio_h, GPIO_Number_3);
}

void led_on(unsigned int led_number)
{
	//if (led_number <= 3)
	GPIO_setLow(gpio_h, (GPIO_Number_e)led_number);
}

void led_off(unsigned int led_number)
{
	//if (led_number <= 3)
	GPIO_setHigh(gpio_h, (GPIO_Number_e)led_number);
}

void led_toggle(unsigned int led_number)
{
	//if (led_number <= 3)
	GPIO_toggle(gpio_h, (GPIO_Number_e)led_number);
}
