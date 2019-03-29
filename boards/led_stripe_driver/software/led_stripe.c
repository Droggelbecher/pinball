
/*#include <string.h> // memset & co*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <checksum.h>

/*#include <checksum.h>*/

#include "led_stripe.h"

#define LEDS 20
struct cRGB led[LEDS];
/*struct cRGB led_off[LEDS];*/

volatile unsigned long milliseconds;

int spi_xfer = 0;

// Command "double buffer" (executing & receiving)
// command_idx denotes the buffer currently being executed, !command_idx the one usable for sending
int command_idx = 0;
Command command_buffer[2];

// Command execution state
int phase = 0;
unsigned long next_action_ms = 0;

void swap_buffers() {
	command_idx = !command_idx;
}

uint8_t* active_buffer() {
	return command_buffer[command_idx];
}

uint8_t* inactive_buffer() {
	return command_buffer[!command_idx];
}

void load(Command c) {
	memcpy(inactive_buffer(), c, sizeof(Command));
	swap_buffers();
}

int main(void) {
	setup();

	clear_leds();
	ws2812_setleds(led, LEDS);

	/*
	Command yellow = {
		.mode = FULL,
		.arg_R0 = 0xff,
		.arg_G0 = 0xa0,
		.arg_B0 = 0x00
	};

	Command mod = {
		.mode = MOD,
		.arg_R0 = 0xff,
		.arg_G0 = 0xa0,
		.arg_B0 = 0x00,
		.arg_MOD = 4 //LEDS / 2 // LEDS/2=one LED, every other value: every Xth LED
	};

	Command gradient2 = {
		.mode = GRADIENT2,
		.arg_R0 = 0x30,
		.arg_G0 = 0x00,
		.arg_B0 = 0x00,

		.arg_R1 = 0x00,
		.arg_G1 = 0x20,
		.arg_B1 = 0x00
	};

	Command rotate = {
		.mode = ROTATE,
		.arg_DT = 10,
		.arg_MOD = 1 // direction
	};

	Command fadeout = {
		.mode = FADEOUT,
		.arg_DT = 10
	};
	*/

	Command gradient = {
		GRADIENT2,
		0x00, 0xf0, 0x00, // color 0
		0x00, // dt (unused)
		0xf0, 0x00, 0x00  // color 1
	};

	Command rotate = {
		ROTATE,
		0x00, 0x00, 0x00, // color0 (unused)
		10, // dt
		1, // mod (=direction)
		0x00, 0x00 // g1, b1 (unused)
	};

	// Command flash = {
	// 	FLASH,
	// 	0x60, 0x60, 0xff, // color 0
	// 	40, // DT
	// 	3, // MOD
	// 	0x00, 0x00
	// };

	load(gradient);
	execute();

	load(rotate);
	execute();

	phase = 0;
	while(1) {
		execute();
		xfer_spi();

	}

	return 0;
}

void clear_leds() {
	memset(&led, 0, LEDS * sizeof(struct cRGB));
}


/**
 * SS interrupt.
 * Activated when SS flanks (low<->high), i.e. at beginning and end of
 * SPI transmission.
 */
ISR(PCINT0_vect) {
	spi_xfer = !(PINB & (1 << PB2)); // SS pin high -> end of transmission
}

void xfer_spi(void) {
	if(!spi_xfer) { return; }

	uint8_t *cmd = command_buffer[!command_idx];

	while(!(SPSR & (1 << SPIF))) { }

	for(int i = 0; i < 8; i++) {
		cmd[i] = SPDR;
	}
	uint8_t chk = SPDR;
	uint8_t mychecksum = checksum(cmd, 8);
	if(chk == mychecksum) {
		// New command received correctly, switch to it
		command_idx = !command_idx;

		if(id(cmd) != id(command_buffer[!command_idx])) {
			// Did command ID change? If yes it means we started a new command (instance)
			phase = 0;
		}
	}

	spi_xfer = 0;
}

void execute() {
	uint8_t *c = active_buffer();
	switch(mode(c)) {
		case FULL:
			for(int i = 0; i < LEDS; i++) {
				led[i].r = r0(c);
				led[i].g = g0(c);
				led[i].b = b0(c);
			}
			break;

		case MOD:
			clear_leds();
			for(int i = 0; i < LEDS / 2; i += mod(c)) {
				led[i].r = r0(c);
				led[i].g = g0(c);
				led[i].b = b0(c);

				led[LEDS - i - 1].r = r0(c);
				led[LEDS - i - 1].g = g0(c);
				led[LEDS - i - 1].b = b0(c);
			}
			break;

		case GRADIENT:
			for(int i = 0; i < LEDS/2; i++) {
				led[i].r = r0(c) + i * (r1(c) - r0(c)) / (LEDS/2 - 1);
				led[i].g = g0(c) + i * (g1(c) - g0(c)) / (LEDS/2 - 1);
				led[i].b = b0(c) + i * (b1(c) - b0(c)) / (LEDS/2 - 1);
				led[LEDS - i - 1] = led[i];
			}
			break;

		case GRADIENT2:
			for(int i = 0; i < LEDS/4; i++) {
				led[i].r = r0(c) + i * (r1(c) - r0(c)) / (LEDS/4 - 1);
				led[i].g = g0(c) + i * (g1(c) - g0(c)) / (LEDS/4 - 1);
				led[i].b = b0(c) + i * (b1(c) - b0(c)) / (LEDS/4 - 1);
				led[LEDS/2 - i - 1] = led[i];
				led[LEDS/2 + i] = led[i];
				led[LEDS - i - 1] = led[i];
			}
			break;

		case ROTATE:
			if(milliseconds >= next_action_ms) {
				next_action_ms = milliseconds + dt(c) * 10;
				
				if(mod(c)) {
					struct cRGB first = led[0];
					memmove(led,              led + 1,      (LEDS/2 - 1) * sizeof(struct cRGB));
					memmove(led + LEDS/2 + 1, led + LEDS/2, (LEDS/2 - 1) * sizeof(struct cRGB));
					led[LEDS/2 - 1] = first;
					led[LEDS/2] = first;
				}
				else {
					struct cRGB last = led[LEDS/2 - 1];
					memmove(led + 1,          led,              (LEDS/2 - 1) * sizeof(struct cRGB));
					memmove(led + LEDS/2,     led + LEDS/2 + 1, (LEDS/2 - 1) * sizeof(struct cRGB));
					led[0] = last;
					led[LEDS - 1] = last;
				}
			}
			break;

		case FADEOUT:
			if(milliseconds >= next_action_ms) {
				next_action_ms = milliseconds + dt(c) * 10;
				for(int i = 0; i < LEDS; i++) {
					if(led[i].r) { led[i].r = (led[i].r - 1) * 0.9; }
					if(led[i].g) { led[i].g = (led[i].g - 1) * 0.9; }
					if(led[i].b) { led[i].b = (led[i].b - 1) * 0.9; }
				}
			}
			break;

		case FLASH:
			if(milliseconds >= next_action_ms && phase/2 <= mod(c)) {
				next_action_ms = milliseconds + dt(c) * 1;
				if(phase % 2 == 0) {
					for(int i = 0; i < LEDS; i++) {
						led[i].r = r0(c);
						led[i].g = g0(c);
						led[i].b = b0(c);
					}
				}
				else {
					clear_leds();
				}
				phase++;
			}
			break;
		default:
			break;
	} // switch

	ws2812_setleds(led, LEDS);
} // execute()

ISR(TIMER1_COMPA_vect)
{
	++milliseconds;
}

void setup(void) {
	// LED data out pin
	DDRC |= (1 << PC0);


	cli();
	// CTC mode, Clock/8
	TCCR1B |= (1 << WGM12) | (1 << CS11);
 
	// Load the high byte, then the low byte
	// into the output compare
	OCR1AH = (CTC_MATCH_OVERFLOW >> 8);
	OCR1AL = (CTC_MATCH_OVERFLOW & 0xff);
 
	// Enable the compare match interrupt
	TIMSK1 |= (1 << OCIE1A);
	sei();
}

