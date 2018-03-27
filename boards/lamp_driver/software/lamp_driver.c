
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

#include "lamp_driver.h"

#define ENABLE_UART 1
#define ENABLE_DEBUG 1

int main(void) {
	setup_spi();
	setup_uart();
	setup_lamps();

	uart_puts("\aLAMP DRIVER 0.1\r\n");

	mainloop();
	/*run_selftest();*/
	return 0;
}

void setup_lamps(void) {
	// Row outputs
	DDRC |= 0x3f; // PC0...PC5 = output
	DDRD |= (1 << PD7);
	DDRB |= (1 << PB1);

	// PB0 = selftest
	PORTB |= (1 << PB0);
	DDRB &= ~(1 << PB0);
	PORTB |= (1 << PB0);

	SR_DDR |= SR_DS | SR_STCP | SR_SHCP;

	memset(lamp_state_buffer, 0, sizeof(lamp_state_buffer));
}

void setup_spi(void) {

	// MISO = output,
	DDRB |= (1 << PB4);

	// SS = input
	DDRB &= ~(1 << PB2);

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

/**
 * If spi_xfer is set (SS low), transfer state of all switches.
 */
inline void xfer_spi(void) {
	if(spi_xfer) {
		/*uart_putc('x');*/
		while(!(SPSR & (1 << SPIF))) { }
		new_lamp_states[0] = SPDR; while(!(SPSR & (1 << SPIF))) { }
		new_lamp_states[1] = SPDR; while(!(SPSR & (1 << SPIF))) { }
		new_lamp_states[2] = SPDR; while(!(SPSR & (1 << SPIF))) { }
		new_lamp_states[3] = SPDR; while(!(SPSR & (1 << SPIF))) { }
		new_lamp_states[4] = SPDR; while(!(SPSR & (1 << SPIF))) { }
		new_lamp_states[5] = SPDR; while(!(SPSR & (1 << SPIF))) { }
		new_lamp_states[6] = SPDR; while(!(SPSR & (1 << SPIF))) { }
		new_lamp_states[7] = SPDR; while(!(SPSR & (1 << SPIF))) { }
		uint8_t chksum = SPDR; //while(!(SPSR & (1 << SPIF))) { }

		uint8_t mychksum = checksum(new_lamp_states, ROWS);
		if(chksum == mychksum) {
#if ENABLE_DEBUG
			uart_putc('.');
#endif
			switch_lamp_state_buffers();
		}
		else {
			/*uart_putc('!');*/
#if ENABLE_DEBUG
			{
				uart_puts("<");
				int i;
				for(i = 0; i < 8; i++) {
					uart_puthex(new_lamp_states[i]);
					uart_putc(' ');
				}
				uart_puts(" , ");
				uart_puthex(chksum);
				uart_puts(" != ");
				uart_puthex(mychksum);
				uart_puts(">\r\n");
			}
#endif
		}

		spi_xfer = 0;
	} // if
} // xfer_spi()

void switch_lamp_state_buffers(void) {
	uint8_t *tmp = lamp_states;
	lamp_states = new_lamp_states;
	new_lamp_states = tmp;
}

void mainloop(void) {
	int i;

	while(1) {
		xfer_spi();
		reset_sr();

		if(!(PINB & (1 << PB0))) {
			run_selftest();
		}

		for(i = 0; i < 8; i++) {
			uint8_t c = lamp_states[i];
			set_column(c);
			_delay_us(5);

			set_column(0);
			_delay_us(1);
			shift_zero();

			/*set_column(0);*/
			/*_delay_us(5);*/
		}

		// 2 "silent rounds" for LED cooldown
		// (datasheets demands a duty cycle of at most 1/10th but we have only
		// 8 LEDs)
		_delay_us(10);

		xfer_spi();
	}
}

void set_column(uint8_t c) {

	// Colum ports:
	// Pin: PC0 PC1 PC2 PC3 PC4 PC5 PB1 PD7
	// Bit:   0   1   2   3   4   5   6   7

	// PORTC = 01cccccc 
	PORTC = (c & 0x3f) | (1 << PC6); // lower 6 bits of c, reset pin (PC6) high

	if(c & (1 << 6)) {
		PORTB |= (1 << PB1);
	}
	else {
		PORTB &= ~(1 << PB1);
	}
	if(c & (1 << 7)) {
		PORTD |= (1 << PD7);
	}
	else {
		PORTD &= ~(1 << PD7);
	}
}


void run_selftest(void) {
	int i;
	int j;
	int k;

	memset(lamp_state_buffer, 0, sizeof(lamp_state_buffer));
	while(1) {
	for(k = 0; k < 8; k++) {

	/*for(l = 0; l < 256; l++) {*/
		lamp_states[k] = k+1;
	}

	for(j = 0; j < 100; j++) {
		reset_sr();

		for(i = 0; i < 8; i++) {
			uint8_t c = lamp_states[i];
			PORTC = (c & 0x3f) | (1 << PC6); // lower 6 bits of c, reset pin (PC6) high

			if(c & (1 << 6)) {
				PORTB |= (1 << PB1);
			}
			else {
				PORTB &= ~(1 << PB1);
			}
			if(c & (1 << 7)) {
				PORTD |= (1 << PD7);
			}
			else {
				PORTD &= ~(1 << PD7);
			}

			shift_zero();
			_delay_us(50);
		}

		// 2 "silent" rounds for LED cooldown
		// (datasheets demands a duty cycle of at most 1/10th but we have only
		// 8 LEDs)
		/*
		shift_zero();
			_delay_us(50);
		shift_zero();
			_delay_us(50);
		*/
		_delay_us(100);
	}

	/*}*/
	}
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


/**
 * Postcondition:
 * Shift register and state register of the 74HC595 are
 * in a state where the first bit is HIGH and all other bits LOW.
 */
void reset_sr(void) {
	uint8_t oldstate = SR_PORT & ~(SR_DS | SR_STCP | SR_SHCP);
	SR_PORT = oldstate;

	// 7 low bits
	int i;
	for(i = 0; i < 7; i++) {
		SR_PORT = oldstate;
		SR_PORT = oldstate | SR_SHCP;
		SR_PORT = oldstate;
	}

	// ... and one high

	// load LOW into first register
	SR_PORT = oldstate | SR_DS;
	SR_PORT = oldstate | SR_DS | SR_SHCP;
	SR_PORT = oldstate | SR_DS;

	// shift register -> output register
	SR_PORT = oldstate ;
	SR_PORT = oldstate | SR_STCP;
}

void shift_one(void) {
	uint8_t oldstate = SR_PORT & ~(SR_DS | SR_STCP | SR_SHCP);
	SR_PORT = oldstate | SR_DS;
	SR_PORT = oldstate | SR_DS | SR_SHCP;
	SR_PORT = oldstate | SR_DS;
	SR_PORT = oldstate | SR_STCP;
}

void shift_zero(void) {
	uint8_t oldstate = SR_PORT & ~(SR_DS | SR_STCP | SR_SHCP);
	SR_PORT = oldstate | 0;
	SR_PORT = oldstate | 0 | SR_SHCP;
	SR_PORT = oldstate | 0;
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



