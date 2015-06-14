
// SPI tutorial:
// http://www.engineersgarage.com/embedded/avr-microcontroller-projects/spi-serial-peripheral-interface-tutorial-circuit
//
//http://www.elecdude.com/2012/12/avr-spi-serial-pheripheral-interface.html
//
// UART:
//https://www.mikrocontroller.net/articles/AVR-GCC-Tutorial/Der_UART

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <string.h> // memset & co

#include "display_module.h"

unsigned char row = 0;
int demo_state = 1;
int demo_frame = 0;
unsigned char pwm_phase = 0;

volatile unsigned int screen_index = 0;

#define ENABLE_UART 0


int selftest = 0;
unsigned long phase = 0;

unsigned int palette[][COLORS] = {
	// -- RED CHANNEL
	// BLACK  RED  GREEN YELLOW ORANGE DRK GRE DRK RE
	{  0x00, 0xff, 0x00,  0x07,  0x1f,   0x00,  0x01, 0x01 },
	// -- GREEN CHANNEL
	{  0x00, 0x00, 0xff,  0x1f,  0x1f,   0x03,  0x00, 0x01 }
};


int main(void) {
	setup_spi();
#if ENABLE_UART
	setup_uart();
#endif

	setup_display();
	clear_screen();
	output_screen();

#if ENABLE_UART
	uart_puts("\aDISPLAY MODULE 0.1\r\n");
#endif

	PORTB |= (1 << PINB0);
	while(1) {
		if(selftest) {
			render_selftest(phase++ >> 6);
		}
		output_screen();
	}
}

void setup_spi(void) {
	// MISO = output,
	// PB1 = output (SS for next module)
	// others input
	DDRB = (1 << PB1) | (1 << PB4);
	// Enable SPI, Slave, set clock rate fck/16, SPI MODE 1
	// http://maxembedded.com/2013/11/the-spi-of-the-avr/
	SPCR = (1<<SPE)|(1<<SPIE); //|(1<<CPHA);

	// INT0 is wired to SS to inform us when a SPI transmission starts
	// so generate an interrupt on falling edge (SS low = transmission
	// running)
	/*MCUR = (1 << ISC01);*/
	/*GICR = (1 << INT0);*/

	// https://sites.google.com/site/qeewiki/books/avr-guide/external-interrupts-on-the-atmega328
	PCICR |= (1 << PCIE0);    // set PCIE0 to enable PCMSK0 scan
	// Generate interrupt on SS pin change
	PCMSK0 |= (1 << PCINT2);

	sei();
}

void enable_next() {
	PORTB &= ~0x02;
}

void disable_next() {
	PORTB |= 0x02;
}

void setup_display(void) {
	// make port C output
	// (will be used to fill the shift registers)
	DDRC = 0xff;
}

void setup_uart(void) {
	UBRR0H = UBRR_VAL >> 8;
	UBRR0L = UBRR_VAL & 0xFF;

	UCSR0B |= (1<<TXEN0);                           // UART TX einschalten
	UCSR0A = (1 << UDRE0);
	UCSR0C = (1<<UCSZ01)|(1 << UCSZ00); // Asynchron 8N1
}

/**
 * SS interrupt.
 * Activated when SS flanks (low<->high), i.e. at beginning and end of
 * SPI transmission.
 * If called on transmission start will handle the complete SPI communication
 * (blockingly), will just return if called at end of transmission.
 */
ISR(PCINT0_vect) {
	int spi_xfer = !(PINB & (1 << PB2)); // SS pin high -> end of transmission
	if(spi_xfer) {
		screen_index = 0;
	}
	else {
		disable_next();
	}
}

ISR(SPI_STC_vect) {
	char ch = SPDR;
	/*if(ch == C_EOT) {*/
		/*screen_index = 0;*/
		/*return;*/
	/*}*/

	if(screen_index >= PIXELS) {
		enable_next();
		return;
	}

	((unsigned char*)(screen[0]))[screen_index] = palette[0][(int)ch];
	((unsigned char*)(screen[1]))[screen_index] = palette[1][(int)ch];
	++screen_index;

}

void uart_putc(char x) {
	// bei neueren AVRs steht der Status in UCSRA/UCSR0A/UCSR1A, hier z.B. fuer ATmega16:
	while (!(UCSR0A & (1<<UDRE0)))  /* warten bis Senden moeglich                   */
	{
	}
	UDR0 = x;
}

void uart_puts(char *s) {
	while(*s) { uart_putc(*(s++)); }
}

void shift(unsigned char x) {
	x = x ? 0b100 : 0b000;

	// SCK=1 SI=x
	PORTC = 0b001 | x;
	// SCK=0 SI=x
	PORTC = 0b000 | x;
}

void shift_row(void) {
	// the comments give the order of pins from QA...QH
	// note that we have to shift them in in the *reverse* order of that
	
	// green col 4 | row 4 | green col 5 | row 5 | ... | green col 7 | row 7
	for(int i = 7; i >= 4; i--) {
		shift(row == i);
		// what we name row is a column for the display (brainfuck!)
		shift(screen[IDX_GREEN][i + (row & 8)][row & 7] & pwm_phase);
	}
	
	// row 3 | green col 3 | ... | row 0 | green col 0
	for(int i = 0; i < 4; i++) {
		shift(screen[IDX_GREEN][i + (row & 8)][row & 7] & pwm_phase);
		shift(row == i);
	}
	
	// red col 4 | row 12 | ... | red col 7 | row 15
	for(int i = 7; i >= 4; i--) {
		shift(row == i + 8);
		shift(screen[IDX_RED][i + (row & 8)][row & 7] & pwm_phase);
	}
	
	// row 11 | red col 3 | ... | row 8 | red col 0
	for(int i = 0; i < 4; i++) {
		shift(screen[IDX_RED][i + (row & 8)][row & 7] & pwm_phase);
		shift(row == i + 8);
	}
}


void output_row(void) {
	if(row >= ROWS) { row = 0; }
	shift_row();
	
	// flank RCK --> write to status registers
	PORTC = 0b010;
	PORTC = 0b000;
	row++;
}

void output_blank_row(void) {
	if(row >= ROWS) { row = 0; }
	for(int i = 0; i < 32; i++) {
		shift(0);
	}
	// flank RCK --> write to status registers
	PORTC = 0b010;
	PORTC = 0b000;

	row++;
}

void output_screen(void) {
	for(int i = 0; i<16; i++) {
		output_row();
	}
	output_blank_row();
	pwm_phase <<= 1;
	if(pwm_phase == 0) {
		pwm_phase = 1;
	}
}

void clear_screen(void) {
	memset(screen, 0, sizeof(screen));
}


void render_selftest(unsigned long phase) {
	clear_screen();

	const int subphases = 64;
	const int modes = 6;


	int mode = (phase / subphases) % modes;
	int subphase = phase % subphases;

	switch(mode) {
		case 0: {
			int row = subphase % 16;
			for(int i = 0; i < 8; i++) {
				screen[IDX_GREEN][row][i] = 0xff;
			}
			break;
		}

		case 1: {
			int row = subphase % 16;
			for(int i = 0; i < 8; i++) {
				screen[IDX_RED][row][i] = 0xff;
			}
			break;
		}

		case 2: {
			int col = subphase % 8;
			for(int i = 0; i < 16; i++) {
				screen[IDX_GREEN][i][col] = 0xff;
			}
			break;
		}

		case 3: {
			int col = subphase % 8;
			for(int i = 0; i < 16; i++) {
				screen[IDX_RED][i][col] = 0xff;
			}
			break;
		}

		case 4: {
			if(subphase % 2) {
				memset(screen, 0xff, sizeof(screen));
			}
			break;
		}

		case 5: {
			memset(screen, 0xff, sizeof(screen));
			break;
		}
	}
}

