#include "global.h"

void main(void)
{
	char cmd = 0;
	uint16_t tmp;

	setup_handles();
	init_system();

	tick_init();
	UART_init();
	leds_init();
	ePWM_init();

	PIE_enable(pie_h);
    CPU_enableGlobalInts(cpu_h);
    CPU_enableDebugInt(cpu_h);

	UART_printf("\r\n\r\n");
	UART_printf(" ### F28027 LaunchPad ; UART to PWM ; by Mikolaj Stawiski ###\r\n\r\n");
	UART_printf("CPU_FREQ_MHZ = %u\r\n", CPU_FREQ_MHZ);

	UART_printf("Press any key to start the code...");
	UART_readByteWait();
	UART_printf("\r\nStarting...\r\n");

	while(1)
	{
		switch (cmd) {
			case 'h':
			{
				UART_printf("Provide High speed PWM divisor (1, 2, 4, 6, 8, 10, 12, 14): ");
				tmp = UART_readUint16Wait(true);
				UART_printf(". ");

				if (ePWM_setClkDiv_hs(tmp)) {
					UART_printf("OK!\r\n");
					ePWM_status_print();
				} else {
					UART_printf("Error\r\n");
				}

				break;
			}
			case 'v':
			{
				UART_printf("Provide PWM divisor (1, 2, 4, 8, 16, 32, 64, 128): ");
				tmp = UART_readUint16Wait(true);
				UART_printf(". ");

				if (ePWM_setClkDiv(tmp)) {
					UART_printf("OK!\r\n");
					ePWM_status_print();
				} else {
					UART_printf("Error\r\n");
				}

				break;
			}
			case 'p':
			{
				UART_printf("Provide PWM period: ");
				tmp = UART_readUint16Wait(true);
				UART_printf(". ");

				if (ePWM_setPeriod(tmp)) {
					UART_printf("OK!\r\n");
					ePWM_status_print();
				} else {
					UART_printf("Error\r\n");
				}

				break;
			}
			case 'c':
			{
				UART_printf("Provide PWM compare: ");
				tmp = UART_readUint16Wait(true);
				UART_printf(".\r\n");

				ePWM_setCompare(tmp);
				ePWM_status_print();

				break;
			}
			case 's':
			{
				if (epwm_period >= 2) {
					ePWM_start();
					ePWM_status_print();
				} else {
					UART_printf("Wrong PWM period!\r\n");
				}

				break;
			}
			case 't':
			{
				ePWM_stop();
				ePWM_status_print();
				break;
			}
			case 'q':
			{
				ePWM_status_print();
				break;
			}
			default:
			{
				UART_printf("\r\n### Commands: ###\r\n");
				UART_printf("\th - High speed PWM divisor (1, 2, 4, 6, 8, 10, 12, 14)\r\n");
				UART_printf("\tv - PWM divisor (1, 2, 4, 8, 16, 32, 64, 128)\r\n");
				UART_printf("\tp - PWM period\r\n");
				UART_printf("\tc - PWM compare\r\n");
				UART_printf("\ts - PWM start\r\n");
				UART_printf("\tt - PWM stop\r\n");
				UART_printf("\tq - PWM status\r\n");
				UART_printf("\r\nUptime = %n seconds\r\n", tick_counter);

				break;
			}
		}

		cmd = UART_readByteWait(); // we read there to show the menu for the first time
	}
}
