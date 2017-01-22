
// SPI tutorial:
// http://www.engineersgarage.com/embedded/avr-microcontroller-projects/spi-serial-peripheral-interface-tutorial-circuit
//
//http://www.elecdude.com/2012/12/avr-spi-serial-pheripheral-interface.html
//
// UART:
//https://www.mikrocontroller.net/articles/AVR-GCC-Tutorial/Der_UART


// TODO:
// * Check pins (P_SIN etc...) which is connected where?
// * Check which pin P_GSCLK must be connected to (Timer!), change breadboard accordingly



#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <string.h> // memset & co

#include "display_module.h"



inline void xfer_spi(void);

int spi_xfer = 0;
int selftest = 1;
unsigned long phase = 0;

unsigned int palette[][COLORS] = {

	// TODO: Fix these color values
	// (they are still PWM patterns!)

	// R     G

	{ 0x00, 0x00 },
	{ 0xff, 0x00 },
	{ 0x00, 0xff },
	{ 0x07, 0x1f },
	{ 0x1f, 0x1f },
	{ 0x00, 0x03 },
	{ 0x01, 0x00 },
	{ 0x01, 0x01 }

};


int main(void) {
	setup_spi();
	setup_tlc5940();
	setup_mosfets();

	clear_screen();

	while(1) {
		if(selftest) {
			render_selftest(phase++);
		}

		output_screen();

		if(!selftest) {
			xfer_spi();
		}
	}
}

void output_screen(void) {

	for(int row = 0; row < 8; row++) {
		output_row(row);
	}
}



inline ScreenIndex decode_master_screen_index(int i) {
	ScreenIndex r;
	r.column = 7 - (i % 8);
	r.row = i / 8;
	return r;
}

inline int encode_lm23088_screen_index(ScreenIndex si) {

	// internal layout:
	// we have 8 anodes (= pairs of rows on the 2 matrices)
	// each 2x 16 columns per board (Green/Red interchangingly)

	return si.row * 16 + si.column * 2 + (si.color == IDX_RED);
}

inline void set_screen(int row, int column, int color, int v) {
	ScreenIndex si = { .row = row, .column = column, .color = color };
	int idx = encode_lm23088_screen_index(si);
	screen[idx] = v;
}

inline int get_screen(int row, int column, int color) {
	ScreenIndex si = { .row = row, .column = column, .color = color };
	int idx = encode_lm23088_screen_index(si);
	return screen[idx]; 
}


inline void xfer_spi(void) {
	if(spi_xfer) {
		int screen_index = 0;

		while(!(SPSR & (1 << SPIF))) { }
		char chx = SPDR;
		(void)chx;

		// read in all but the last byte
		// after the last byte we need to quickly activate the next display
		// module (by pulling its SS pin low), so 
		// we want to avoid the loop overhead there

		for( ; screen_index != PIXELS - 1; ++screen_index) {

			// the nop is for timing optimization
			asm volatile ("nop");
			while(!(SPSR & (1 << SPIF))) { }

			char ch = SPDR;

			ScreenIndex idx = decode_master_screen_index(screen_index);
			idx.color = IDX_GREEN;
			screen[ encode_lm23088_screen_index(idx) ] = palette[IDX_GREEN][(int)ch];
			idx.color = IDX_RED;
			screen[ encode_lm23088_screen_index(idx) ] = palette[IDX_RED][(int)ch];

		}

		// the nop is for timing optimization
		asm volatile ("nop");
		while(!(SPSR & (1 << SPIF))) { }
		char ch = SPDR;

		// enable_next(), pull SS pin of next module low
		// so following bytes will be passed through to it
		enable_next();

		ScreenIndex idx = decode_master_screen_index(screen_index);
		idx.color = IDX_GREEN;
		screen[ encode_lm23088_screen_index(idx) ] = palette[IDX_GREEN][(int)ch];
		idx.color = IDX_RED;
		screen[ encode_lm23088_screen_index(idx) ] = palette[IDX_RED][(int)ch];

		spi_xfer = 0;
	}
}

void enable_next() {
	PORTB &= ~0x02;
}

void disable_next() {
	PORTB |= 0x02;
}

void setup_spi(void) {
	// MISO = output,
	// PB1 = output (SS for next module)
	// others input
	DDRB = (1 << PB1) | (1 << PB4);

	// debug pins
	DDRD |= (1 << PD7) | (1 << PD5) | (1 << PD4);
	PORTD &= ~((1 << PD7) | (1 << PD5) | (1 << PD4));


	// Enable SPI, Slave, set clock rate fck/16, SPI MODE 1
	// http://maxembedded.com/2013/11/the-spi-of-the-avr/
	SPCR = (1<<SPE); //|(1<<SPIE); //|(1<<CPHA);

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

void setup_tlc5940(void) {
	DDR_TLC5940 |= (1 << P_SIN) | (1 << P_SCLK) | (1 << P_XLAT) | (1 << P_BLANK) | (1 << P_GSCLK);

	// Timer 0 (8 Bit)
	TCCR0A = (1 << WGM01) | (0 << WGM00); // CTC
	TCCR0A |= (0 << COM0A1) | (1 << COM0A0); // Toggle on Compare Match
	TCCR0B = (0 << CS02) | (0 << CS01) | (1 << CS00); // No prescaler
	OCR0A = 0; // f(OCR) = F_CPU/2/Prescaler
}

void setup_mosfets(void) {
	DDR_MOSFETS |= 0xff;
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
	if(!spi_xfer) {
		disable_next();
	}
}



#define GETBIT(V, B) ((V >> B) & 0x01)

void output_row(int row) {

	PORT_TLC5940 |= (1 << P_BLANK);

	for(int i = 0; i < COLUMNS * COLORS * 2; i++) {
		// highest 4 MSBits are 0

		PORT_TLC5940 = (1 << P_BLANK); PORT_TLC5940 = (1 << P_BLANK) | (1 << P_SCLK);
		PORT_TLC5940 = (1 << P_BLANK); PORT_TLC5940 = (1 << P_BLANK) | (1 << P_SCLK);
		PORT_TLC5940 = (1 << P_BLANK); PORT_TLC5940 = (1 << P_BLANK) | (1 << P_SCLK);
		PORT_TLC5940 = (1 << P_BLANK); PORT_TLC5940 = (1 << P_BLANK) | (1 << P_SCLK);

		unsigned char s = get_screen(row, i / 2, i % 2);
		unsigned char b = GETBIT(s, 0) << P_SIN;

		PORT_TLC5940 = (1 << P_BLANK) | b;
		PORT_TLC5940 |= (1 << P_SCLK);

		b = GETBIT(s, 1) << P_SIN;
		PORT_TLC5940 = (1 << P_BLANK) | b;
		PORT_TLC5940 |= (1 << P_SCLK);

		b = GETBIT(s, 2) << P_SIN;
		PORT_TLC5940 = (1 << P_BLANK) | b;
		PORT_TLC5940 |= (1 << P_SCLK);

		b = GETBIT(s, 3) << P_SIN;
		PORT_TLC5940 = (1 << P_BLANK) | b;
		PORT_TLC5940 |= (1 << P_SCLK);

		b = GETBIT(s, 4) << P_SIN;
		PORT_TLC5940 = (1 << P_BLANK) | b;
		PORT_TLC5940 |= (1 << P_SCLK);

		b = GETBIT(s, 5) << P_SIN;
		PORT_TLC5940 = (1 << P_BLANK) | b;
		PORT_TLC5940 |= (1 << P_SCLK);

		b = GETBIT(s, 6) << P_SIN;
		PORT_TLC5940 = (1 << P_BLANK) | b;
		PORT_TLC5940 |= (1 << P_SCLK);

		b = GETBIT(s, 7) << P_SIN;
		PORT_TLC5940 = (1 << P_BLANK) | b;
		PORT_TLC5940 |= (1 << P_SCLK);
	}

	PORT_TLC5940 = (1 << P_BLANK) | (1 << P_XLAT);
	PORT_TLC5940 = (1 << P_BLANK);

	PORT_TLC5940 = 0; // unblank

	// Reactivate GSCKL and reset TCNT
	TCCR0A = (1 << WGM01) | (0 << WGM00);
	TCCR0A |= (0 << COM0A1) | (1 << COM0A0);
	TCNT1 = 0;

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
				set_screen(row, i, IDX_GREEN, 0xff);
			}
			break;
		}

		case 1: {
			int row = subphase % 16;
			for(int i = 0; i < 8; i++) {
				set_screen(row, i, IDX_RED, 0xff);
			}
			break;
		}

		case 2: {
			int col = subphase % 8;
			for(int i = 0; i < 16; i++) {
				set_screen(i, col, IDX_GREEN, 0xff);
			}
			break;
		}

		case 3: {
			int col = subphase % 8;
			for(int i = 0; i < 16; i++) {
				set_screen(i, col, IDX_RED, 0xff);
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

