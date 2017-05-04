/*
 * tick.c
 *
 *  Created on: 26 kwi 2017
 *      Author: miki
 */

#include "tick.h"

TIMER_Handle tick_timer;
volatile uint32_t tick_counter;

void tick_init(void)
{
	tick_counter = 0;
	tick_timer = TIMER_init((void *)TIMER2_BASE_ADDR, sizeof(TIMER_Obj));

	PIE_registerSystemIntHandler(pie_h, PIE_SystemInterrupts_TINT2, (intVec_t)&tick_isr);

	TIMER_stop(tick_timer);
	TIMER_setPeriod(tick_timer, 60 * 1000000); // 1s tick

    TIMER_setPreScaler(tick_timer, 0);
    TIMER_reload(tick_timer);
    TIMER_setEmulationMode(tick_timer, TIMER_EmulationMode_StopAfterNextDecrement);
    TIMER_enableInt(tick_timer);

    CPU_enableInt(cpu_h, CPU_IntNumber_14); // timer 2 interrupt

    TIMER_start(tick_timer);
}

interrupt void tick_isr(void)
{
	led_toggle(LED_TICK);
	tick_counter++;
}
