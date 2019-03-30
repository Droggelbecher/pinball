
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
struct cRGB led_off[LEDS];

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
		COLOR_MOD | ANIM_ROTATE,
		4, // modulus
		0x00, 0x00, 0xf0, // color 0
		0xf0, 0x10, 0x00, // color 1
		0x01, // direction
		30, // dt
		3, // count (unused)
	};

	// Command flash = {
	// 	FLASH,
	// 	0x60, 0x60, 0xff, // color 0
	// 	40, // DT
	// 	3, // MOD
	// 	0x00, 0x00
	// };

	load(gradient);
	start_execute();

	/*load(rotate);*/
	/*execute();*/

	/*phase = 0;*/
	while(1) {
		execute();
		/*xfer_spi();*/

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
			/*phase = 0;*/
			start_execute();
		}
	}

	spi_xfer = 0;
}

void mirror_leds() {
	for(int i = 0; i < LEDS / 2; i++) {
		led[LEDS - i - 1].r = led[i].r;
		led[LEDS - i - 1].g = led[i].g;
		led[LEDS - i - 1].b = led[i].b;
	}
}

void start_execute() {
	uint8_t *c = active_buffer();

	clear_leds();
	phase = 0;

	switch(color_mode(c)) {
		case COLOR_MOD:
			for(int i = 0; i < LEDS / 2; i++) {
				if(i % mod(c) == 0) {
					led[i].r = r0(c); led[i].g = g0(c); led[i].b = b0(c);
				}
				else {
					led[i].r = r1(c); led[i].g = g1(c); led[i].b = b1(c);
				}
			}
			mirror_leds();
			break;

		case COLOR_GRADIENT: {
			int n = (LEDS/2) / (mod(c) + 1);
			for(int i = 0; i < n; i++) {
				for(int offs = 0; offs < LEDS/2; offs += 2 * n) {
					led[i].r = r0(c) + i * (r1(c) - r0(c)) / (n - 1);
					led[i].g = g0(c) + i * (g1(c) - g0(c)) / (n - 1);
					led[i].b = b0(c) + i * (b1(c) - b0(c)) / (n - 1);

					if(offs + 2*n <= LEDS/2) {
						led[offs + 2*n - i - 1] = led[i];
					}
				}
			}
			mirror_leds();
			break;
		}

		default:
			break;
	}
	ws2812_setleds(led, LEDS);
}

void execute() {
	uint8_t *c = active_buffer();
	if(dt(c) == 0) {
		return;
	}

	switch(anim_mode(c)) {
		case ANIM_ROTATE:
			if(milliseconds >= next_action_ms) {
				next_action_ms = milliseconds + dt(c) * 10;
				
				if(dir(c)) {
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
			ws2812_setleds(led, LEDS);
			break;

		case ANIM_FADEOUT:
			if(milliseconds >= next_action_ms) {
				next_action_ms = milliseconds + dt(c) * 10;
				for(int i = 0; i < LEDS; i++) {
					if(led[i].r) { led[i].r = (led[i].r - 1) * 0.9; }
					if(led[i].g) { led[i].g = (led[i].g - 1) * 0.9; }
					if(led[i].b) { led[i].b = (led[i].b - 1) * 0.9; }
				}
			}
			ws2812_setleds(led, LEDS);
			break;

		case ANIM_FLASH:
			if(milliseconds >= next_action_ms && phase/2 <= count(c)) {
				next_action_ms = milliseconds + dt(c) * 1;
				if(phase % 2 == 0) {
					ws2812_setleds(led, LEDS);
				}
				else {
					ws2812_setleds(led_off, LEDS);
				}
				phase++;
			}
			break;
		default:
			break;
	} // switch

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

