
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

// In ms, values over 10 risk damage to the LED matrix
#define PULSE_TIME 1.0

#define FRAME_RATE (1.0 / (8.0 * (PULSE_TIME / 1000.0)))


inline void xfer_spi(void);

int spi_xfer = 0;
int selftest = 0;
unsigned long phase = 0;

unsigned int palette[][COLORS] = {

	// R     G

	{ 0x00, 0x00 }, // Black
	{ 0xff, 0x00 }, // Full red
	{ 0x00, 0xff }, // Full green
	{ 0x40, 0x80 },
	{ 0x80, 0x80 },
	{ 0x00, 0x40 },
	{ 0x10, 0x00 },
	{ 0x10, 0x10 }

};


int main(void) {
	//cli();
	setup_spi();
	setup_tlc5940();
	setup_mosfets();
	sei();

	clear_screen();

	double p = 0.0;

	while(1) {
		if(selftest) {
			p += 10.0 * (1.0 / FRAME_RATE);
			if(p >= 1.0) {
				phase += (int)p;
				p = 0;
			}
			render_selftest(phase);
		}

		output_screen();

		if(!selftest) {
			xfer_spi();
		}
	}
}

void output_screen(void) {

	for(int row = 0; row < 8; row++) {
		output_column(row);
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

	//return si.column * 32 + si.row * 2 + (si.color == IDX_RED);
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
	PORTB &= ~(1 << PB0);
}

void disable_next() {
	PORTB |= (1 << PB0);
}

void setup_spi(void) {
	// MISO = output,
	// PB0 = output (SS for next module)
	// others input
	DDRB |= (1 << PB0) | (1 << PB4);

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

}

void setup_tlc5940(void) {
	DDR_TLC5940 |= (1 << P_SIN) | (1 << P_SCLK) | (1 << P_XLAT) | (1 << P_BLANK);

	// Timer sources:
	// https://sites.google.com/site/qeewiki/books/avr-guide/timers-on-the-atmega328
	// https://www.mikrocontroller.net/articles/AVR-GCC-Tutorial/Die_Timer_und_Z%C3%A4hler_des_AVR
	// http://www.openmusiclabs.com/learning/digital/synchronizing-timers/
	// https://maxembedded.wordpress.com/2011/06/24/avr-timers-timer0-2/

	// User timer0 for timing BLANK duration
	// Use timer1 for GSCLK
	//
	DDRB |= (1 << PB1); // = OC1, will be toggled by TIMER1

	// We want 256 cycles of TIMER1, with prescaler=1 that is 256*125ns = 32us
	TCCR0A = 0; //(1 << WGM01); // CTC, no output pins
	TCCR1A = (0 << COM1A1) | (1 << COM1A0) // Toggle Pin OC1(=PB1) on compare match
		| (0 << WGM10) | (0 << WGM11); // Disable PWM mode

	TCNT0 = 0;
	TCNT1 = 0;
	//OCR0A = 255;
	OCR1A = 0; // compare value

	TCCR0B = (0 << CS02) | (0 << CS01) | (1 << CS00); // Prescaler = 1
	TCCR1B = (0 << ICNC1) | (0 << ICES1) // No input capture noice cancelling
		| (1 << WGM12) // CTC = Reset counter after match
		| (0 << CS12) | (0 << CS11) | (1 << CS10); // Prescaler = 1

	//TIFR |= (1 << OCF1A);
	//TIMSK1 |= (1 << OCIE1A); // Enable timer compare match interrupt
	//GTCCR &= ~(1 << TSM); // continue all timers
	//GTCCR = 0; // continue all timers
	//GTCCR |= (1 << TSM); // freeze timers for now
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

void output_column(int column) {

	PORT_TLC5940 |= (1 << P_BLANK);

	for(int matrix = 0; matrix < 2; matrix++)
	for(int row = 0; row < ROWS; row++)
	for(int color = 0; color < COLORS; color++) {
		// highest 4 MSBits are 0

		PORT_TLC5940 = (1 << P_BLANK); PORT_TLC5940 = (1 << P_BLANK) | (1 << P_SCLK);
		PORT_TLC5940 = (1 << P_BLANK); PORT_TLC5940 = (1 << P_BLANK) | (1 << P_SCLK);
		PORT_TLC5940 = (1 << P_BLANK); PORT_TLC5940 = (1 << P_BLANK) | (1 << P_SCLK);
		PORT_TLC5940 = (1 << P_BLANK); PORT_TLC5940 = (1 << P_BLANK) | (1 << P_SCLK);

		unsigned char s = get_screen((1 - matrix) * 8 + row, column, color);
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

	// Latch
	PORT_TLC5940 = (1 << P_BLANK);
	PORT_TLC5940 = (1 << P_BLANK) | (1 << P_XLAT);
	PORT_TLC5940 = (1 << P_BLANK);

	// Additional SCLK cycle at the end
	PORT_TLC5940 = (1 << P_BLANK) | (1 << P_SCLK);

	PORT_TLC5940 = 0; // unblank

	PORT_MOSFETS = ~(1 << column);

	//GTCCR &= ~(1 << TSM); // continue timers
	//_delay_ms(5.0); // takes ~30 times too long when timer1 is running full speed

	// GSCLK Phase
	// ===========

	// TIMER1 controls GSCLK pin
	// TIMER0 counts the number of activations
	// At 16MHz an activation will be 125ns,
	// 256 of those will take about 32us (plus overhead).
	// After that we need to toggle P_BLANK to start another PWM cycle in the TLC5940.
	// 
	// We want to spend about 5ms doing this so we need 5000/32 = 156.25 ~= 156 repetitions
	// of this (again, loop overhead will slow this down, which is ok,
    // as long as were below 10ms a precise timing is not important here)

	int cycles = (int)(PULSE_TIME / 0.032);
	for(int i = 0; i < cycles; i++) {

		// one GSCLK period takes 2 timer ticks, so wait 2*256 ticks

		for(int j = 0; j < 2; j++) {
			TCNT0 = 0;
			// Reset TOV0 to 0 by writing a one to it (don't ask, its technical...)
			// http://www.atmel.com/webdoc/AVRLibcReferenceManual/FAQ_1faq_intbits.html
			TIFR0 = (1 << TOV0);
			while(!(TIFR0 & (1 << TOV0))) { // wait for overflow event
			}
		}

		PORT_TLC5940 = (1 << P_BLANK);
		PORT_TLC5940 = 0;
	}


	PORT_MOSFETS = 0xff;
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
		// green row wandering down
		case 0: {
			int row = subphase % 16;
			for(int i = 0; i < 8; i++) {
				set_screen(row, i, IDX_GREEN, 0xff);
			}
			break;
		}

		// red row wandering down
		case 1: {
			int row = subphase % 16;
			for(int i = 0; i < 8; i++) {
				set_screen(row, i, IDX_RED, 0xff);
			}
			break;
		}

		// green column left to right
		case 2: {
			int col = subphase % 8;
			for(int i = 0; i < 16; i++) {
				set_screen(i, col, IDX_GREEN, 0xff);
			}
			break;
		}

		// red row left to right
		case 3: {
			int col = subphase % 8;
			for(int i = 0; i < 16; i++) {
				set_screen(i, col, IDX_RED, 0xff);
			}
			break;
		}

		// full blink
		case 4: {
			if(subphase % 2) {
				memset(screen, 0xff, sizeof(screen));
			}
			break;
		}

		// full light
		case 5: {
			memset(screen, 0xff, sizeof(screen));
			break;
		}
	}
}

