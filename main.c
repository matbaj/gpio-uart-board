/* gpio-uart AVR part
 *
 *  Documentatiom:
 *  Frame:
 *  _ _ _ _ O A A A V
 *
 *  O - Operation 1-set 0-read
 *  A - Address
 *  V - Value (only used for write)
 */


#define F_CPU 16000000UL
#define BAUD 2000000
#include <util/setbaud.h>
#include <avr/io.h>
#include <avr/interrupt.h>
unsigned char c;
unsigned char addr;
unsigned char ret;
static int uart_putchar(char c)
{
	while (!(UCSR0A&0x20));
	UDR0 = c;
	return 0;
}

static int uart_getchar(void)
{
	while (!(UCSR0A&0x80));
	c = UDR0;
	addr = c>>1;
	addr = addr&0x7;
	if (c&0x10)	{
		/* write */
		if (c&0x1)
			PORTB |= _BV(addr);
		else
			PORTB &= ~_BV(addr);
	} else {
		/* read */
		ret = PINB & _BV(addr) ? 1 : 0;
		uart_putchar(ret);
	}
	/* uart_putchar(c); */
	return c;
}

void uartInit(void)
{
	UBRR0L = UBRRL_VALUE;
	UBRR0H = UBRRH_VALUE;
#if USE_2X
	UCSR0A |= _BV(U2X0);
#else
	UCSR0A &= ~(_BV(U2X0));
#endif
	 UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */
	 UCSR0B = _BV(RXEN0) | _BV(TXEN0);   /* Enable RX and TX */
}

int main(void)
{
	DDRB |= _BV(DDB5);
	uartInit();
	while (1)
		uart_getchar();
}
