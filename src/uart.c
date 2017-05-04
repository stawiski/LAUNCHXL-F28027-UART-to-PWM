/*
 * uart.c
 *
 *  Created on: 26 kwi 2017
 *      Author: miki
 */

#include "uart.h"

//struct UART_data_chunk_s g_UART_data_chunk;

void UART_init(void)
{
	CLK_enableSciaClock(clk_h); // enable SCI peripheral

	//PIE_registerPieIntHandler(pie_h, PIE_GroupNumber_9, PIE_SubGroupNumber_2, (intVec_t)&UART_TXRDY_ISR); // enable SCI TXRDY interrupt
    //PIE_enableInt(pie_h, PIE_GroupNumber_9, PIE_InterruptSource_SCIATX);
	//CPU_enableInt(cpu_h, CPU_IntNumber_9);

	// set GPIOs for the TX and RX pins
	GPIO_setMode(gpio_h, GPIO_Number_28, GPIO_28_Mode_SCIRXDA); // GPIO28 as SCI RX
	GPIO_setMode(gpio_h, GPIO_Number_29, GPIO_29_Mode_SCITXDA); // GPIO29 as SCI TX

	// SCI Communication Control Register
	sci->SCICCR =
			(0 << 7) | // One stop bit
			(0 << 5) | // Parity disabled
			(0 << 4) | // Loop Back test mode disabled
			(0 << 3) | // Idle-line mode protocol selected
			(7 << 0);  // Character-length (8 bits)

	sci->SCICTL1 =
			(0 << 6) | // Receive error interrupt disabled
			(1 << 5) | // After a system reset, re-enable the SCI by writing a 1 to this bit.
			(0 << 3) | // TXWAKE (not used)
			(0 << 2) | // Sleep mode disabled
			(1 << 1) | // enable transmitter
			(1 << 0);  // enable receiver

	// Highest baud rate possible (when baud == 0 then baud = LSPCLK/16)
	// Baud = 60MHz/16 = 3.75MHz
	sci->SCIHBAUD = 0;
	sci->SCILBAUD = 0;

	//sci->SCICTL2 = (1 << 0); // Enable UART_TXRDY_ISR interrupt
}

interrupt void UART_TXRDY_ISR(void) // configured for TXRDY interrupt
{
	/*
	ENABLE_PROTECTED_REGISTER_WRITE_MODE;
	gpio->GPACLEAR = (uint32_t)1 << 1; // set pin 1 low
	gpio->GPASET = (uint32_t)1 << 0; // set pin 0 high

	PIE_clearInt(pie_h, PIE_GroupNumber_9); // clear PIE interrupt flag
	sci->SCITXBUF = '*'; // clear the flag and send the data

	gpio->GPACLEAR = (uint32_t)1 << 0; // set pin 0 low
	gpio->GPASET = (uint32_t)1 << 1; // set pin 1 high
	DISABLE_PROTECTED_REGISTER_WRITE_MODE;
	return;
	*/
}

/*
	This part of the code was written by oPossum (Source: http://www.43oh.com/forum/viewtopic.php?f=10&t=1732):

	UART_xtoa()
	UART_puth()
	UART_printf()

	Thanks for sharing!
*/

static const unsigned long dv[] =
{
//  4294967296      // 32 bit unsigned max
		1000000000,// +0
		100000000, // +1
		10000000, // +2
		1000000, // +3
		100000, // +4
//       65535      // 16 bit unsigned max
		10000, // +5
		1000, // +6
		100, // +7
		10, // +8
		1, // +9
		};

static void UART_xtoa(unsigned long x, const unsigned long *dp)
{
	char c;
	unsigned long d;
	if (x) {
		while (x < *dp) ++dp;
		do {
			d = *dp++;
			c = '0';
			while (x >= d) ++c, x -= d;
			UART_putc(c);
		} while (!(d & 1));
	} else
		UART_putc('0');
}

static void UART_puth(unsigned n)
{
	static const char hex[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	UART_putc(hex[n & 15]);
}

void UART_printf(char *format, ...)
{
	char c;
	int i;
	long n;

	va_list a;
	va_start(a, format);
	while(c = *format++) {
		if(c == '%') {
			switch(c = *format++) {
				case 's': // String
					UART_puts(va_arg(a, char*));
					break;
				case 'c':// Char
					UART_putc(va_arg(a, char));
				break;
				case 'i':// 16 bit Integer
				case 'u':// 16 bit Unsigned
					i = va_arg(a, int);
					if(c == 'i' && i < 0) i = -i, UART_putc('-');
					UART_xtoa((unsigned)i, dv + 5);
				break;
				case 'l':// 32 bit Long
				case 'n':// 32 bit uNsigned loNg
					n = va_arg(a, long);
					if(c == 'l' && n < 0) n = -n, UART_putc('-');
					UART_xtoa((unsigned long)n, dv);
				break;
				case 'x':// 16 bit heXadecimal
					i = va_arg(a, int);
					UART_puth(i >> 12);
					UART_puth(i >> 8);
					UART_puth(i >> 4);
					UART_puth(i);
				break;
				case 0: return;
				default: goto bad_fmt;
			}
		} else
			bad_fmt: UART_putc(c);
	}
	va_end(a);
}

void UART_puts(char *s)
{
	char c;
	while (c = *s++) { // Loops through each character in string 's' till null (end of string) character
		UART_sendByte(c);
	}
}

void UART_putc(unsigned char byte)
{
	UART_sendByte(byte);
}

void UART_sendByte(unsigned char byte)
{
	/*
	while ((sci->SCIFFTX >> 8) & 0x1F); // wait while SCI is busy (transmit FIFO not empty)
	*/

	while (!(sci->SCICTL2 & (1 << 7))) kick_watchdog(); // Transmitter buffer register ready flag - wait for bit to set (SCITXBUF is ready to receive the next character)
	sci->SCITXBUF = byte;
}

unsigned char UART_readByte(void)
{
	while (!(sci->SCIRXST & (1 << 6))) kick_watchdog(); // SCI receiver-ready flag, wait for "Character ready to be read from SCIRXBUF"
	return (sci->SCIRXBUF & 0xFF);
}

unsigned char UART_readByteWait(void)
{
	unsigned char c;

	kick_watchdog();
	while ((c = UART_readByte()) == 0) { // keep calling UART_readByte() until it returns non-zero value
		kick_watchdog();
	}

	return c;
}

uint16_t UART_readUint16Wait(bool echo_back)
{
	uint16_t val;
	char c;

	val = 0;

	while ((c = UART_readByteWait()) != 13) { // loop till "enter" is sent
		if (c >= 48 && c <= 57) { // process only numerical (0...9) characters
			val = (val*10) + (c - 48); // add this number to the value
			if (echo_back) {
				UART_putc(c); // send character back if echo_back is enabled
			}
		}
	}

	return val;
}

void UART_print_unit(uint32_t value)
{
	if (value > 10000) {
		value = value / 1000;

		if (value > 10000) {
			value = value / 1000;
			UART_printf("%n M", value); // Mega
		} else {
			UART_printf("%n k", value); // kilo
		}
	} else {
		UART_printf("%n ", value); // nothing
	}
}
