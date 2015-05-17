
// SPI tutorial:
// http://www.engineersgarage.com/embedded/avr-microcontroller-projects/spi-serial-peripheral-interface-tutorial-circuit
//
//http://www.elecdude.com/2012/12/avr-spi-serial-pheripheral-interface.html
//
// UART:
//https://www.mikrocontroller.net/articles/AVR-GCC-Tutorial/Der_UART

#include <string.h> // memset & co

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <checksum.h>

#include "switches_module.h"

#define ENABLE_UART 0
#define ENABLE_DEBUG 0


uint8_t spi_current_column = 0;
volatile uint8_t spi_xfer = 0;

int main(void) {
	memset(debounce_state, 0, sizeof(debounce_state) * sizeof(debounce_state[0]));
	memset(debounce_alpha, 63, sizeof(debounce_alpha) * sizeof(debounce_alpha[0]));

	setup_spi();
	setup_uart();
	setup_switches();
	uart_puts("\aSWITCHES MODULE 0.1\r\n");
	mainloop();
	return 0;
}

/**
 * If spi_xfer is set (SS low), transfer state of all switches.
 */
inline void xfer_spi(void) {
	int i;

	if(spi_xfer) {
		PORTD |= (1 << PD5);

		for(i = 0; spi_xfer; i++) {
			char c = SPDR;
			while(!(SPSR & (1 << SPIF))) {
				if(!spi_xfer) {
					// SS high -> abort, abort!
					PORTD &= ~(1 << PD5);
					return;
				}
			}
			if(i < sizeof(switches_states)) {
				SPDR = switches_states[i];
			}
			else if(i == sizeof(switches_states)) {
				SPDR = checksum(switches_states, sizeof(switches_states));
			}
			else {
				// Master requests  too many bytes from us?!
				SPDR = 0x55;
			}
		} // for
	} // if
} // xfer_spi()

void mainloop(void) {
	int i;

	while(1) {
		xfer_spi();
		reset_sr();

		// rows 0...5 = PC0...PC5
		// PC0 | .. | PC5 = 0b11111 = 0x1f
		// rows 6,7 = PB0,PB1

		for(i = 0; i < 8; i++) {
			xfer_spi();
			uint8_t col_state = (PINC & 0x3f) | ((PINB & 0x03) << 6);
			int j;
			for(j = 0; j < 8; j++) {
				xfer_spi();

				uint8_t bit = ((col_state >> j) & 1);

				debounce_state[i][j] =
					// bit*255 * alpha/255
					// = bit * alpha
					bit * debounce_alpha[i][j] + 

					// oldval * (1 - alpha/255)
					// ~= oldval * (255 - alpha)/256
					((uint16_t)debounce_state[i][j] * (uint16_t)(0xff - debounce_alpha[i][j])) / 256;
			}
			advance_sr(1);

			xfer_spi();

			switches_states[i] =
				(debounce_state[i][0] >= 0x80) << 0 |
				(debounce_state[i][1] >= 0x80) << 1 |
				(debounce_state[i][2] >= 0x80) << 2 |
				(debounce_state[i][3] >= 0x80) << 3 |
				(debounce_state[i][4] >= 0x80) << 4 |
				(debounce_state[i][5] >= 0x80) << 5 |
				(debounce_state[i][6] >= 0x80) << 6 |
				(debounce_state[i][7] >= 0x80) << 7;

			xfer_spi();
		}

	} // while
}


/**
 * SS interrupt.
 * Activated when SS flanks (low<->high), i.e. at beginning and end of
 * SPI transmission.
 * If called on transmission start will handle the complete SPI communication
 * (blockingly), will just return if called at end of transmission.
 */
ISR(PCINT0_vect) {
	spi_xfer = !(PINB & (1 << PB2)); // SS pin high -> end of transmission
}


void setup_spi(void) {

	// MISO = output,
	DDRB |= (1 << PB4);

	// SS = input
	DDRB &= ~(1 << PB2);

	// ERROR LED
	DDRD |= (1 << PD7) | (1 << PD5);
	PORTD &= ~((1 << PD7) | (1 << PD5));

	// Enable SPI, Slave, set clock rate fck/16, SPI MODE 0
	// http://maxembedded.com/2013/11/the-spi-of-the-avr/
	/*SPCR = (1<<SPE)|(1<<SPIE); //|(1<<CPHA);*/
	SPCR = (1<<SPE); //|(1<<CPHA);

	char c;
	c = SPSR;
	c = SPDR;
	SPDR = c;

	// https://sites.google.com/site/qeewiki/books/avr-guide/external-interrupts-on-the-atmega328
	PCICR |= (1 << PCIE0);    // set PCIE0 to enable PCMSK0 scan
	// Generate interrupt on SS pin change
	PCMSK0 |= (1 << PCINT2);

	sei();
}

void setup_uart(void) {
#if ENABLE_UART
	UBRR0H = UBRR_VAL >> 8;
	UBRR0L = UBRR_VAL & 0xFF;

	UCSR0B |= (1<<TXEN0);                           // UART TX einschalten
	UCSR0A = (1 << UDRE0);
	UCSR0C = (1<<UCSZ01)|(1 << UCSZ00); // Asynchron 8N1
#endif
}

void setup_switches(void) {
	// PD2..PD4 output (column control via shift register)
	// PD5, PD6 input (rows 6,7)
	/*DDRD = PD2 | PD3 | PD4;*/
	SR_DDR |= SR_DS | SR_STCP | SR_SHCP;

	DDRC &= ~0x3f; // PC0..5 = input for rows 0..5
	DDRB &= ~0x03; // PB0,1 = input for rows 6,7
}

/**
 * Postcondition:
 * Shift register and state register of the 74HC595 are
 * in a state where the first bit is HIGH and all other bits LOW.
 */
void reset_sr(void) {
	uint8_t oldstate = SR_PORT & ~(SR_DS | SR_STCP | SR_SHCP);
	SR_PORT = oldstate;

	// 7 high bits
	int i;
	for(i = 0; i < 7; i++) {
		SR_PORT = oldstate | SR_DS;
		SR_PORT = oldstate | SR_DS | SR_SHCP;
		SR_PORT = oldstate | SR_DS;
	}

	// ... and one low

	// load LOW into first register
	SR_PORT = oldstate ;
	SR_PORT = oldstate | SR_SHCP;
	SR_PORT = oldstate ;

	// shift register -> output register
	SR_PORT = oldstate ;
	SR_PORT = oldstate | SR_STCP;
}


/**
 * Advance Shift register by one (SR <<= 1)
 */
void advance_sr(uint8_t colbit) {
	uint8_t oldstate = SR_PORT & ~(SR_DS | SR_STCP | SR_SHCP);
	
	uint8_t x = colbit ? SR_DS : 0;

	SR_PORT = oldstate | x;
	SR_PORT = oldstate | x | SR_SHCP;
	SR_PORT = oldstate | x;

	SR_PORT = oldstate | SR_STCP;
}

void uart_putc(char x) {
#if ENABLE_UART
	// bei neueren AVRs steht der Status in UCSRA/UCSR0A/UCSR1A, hier z.B. fuer ATmega16:
	while (!(UCSR0A & (1<<UDRE0)))  /* warten bis Senden moeglich                   */
	{
	}
	UDR0 = x;
#endif
}

void uart_puts(char *s) {
#if ENABLE_UART
	while(*s) { uart_putc(*(s++)); }
#endif
}

void uart_puthex(uint8_t x) {
#if ENABLE_UART
	uart_putnibble(x >> 4);
	uart_putnibble(x & 0x0f);
#endif
}

void uart_putnibble(uint8_t x) {
#if ENABLE_UART
	uart_putc(x < 10 ? '0' + x : 'a' + x - 10);
#endif
}



