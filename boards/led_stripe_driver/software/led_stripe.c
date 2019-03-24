
/*#include <string.h> // memset & co*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>

/*#include <checksum.h>*/

#include "led_stripe.h"

#define LEDS 20
struct cRGB led[LEDS];
/*struct cRGB led_off[LEDS];*/

volatile unsigned long milliseconds;

// Command execution state
int phase = 0;
unsigned long next_action_ms = 0;

int main(void) {
	setup();

	clear_leds();
	ws2812_setleds(led, LEDS);

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

	Command gradient = {
		.mode = GRADIENT,
		.arg_R0 = 0x00,
		.arg_G0 = 0xa0,
		.arg_B0 = 0x30,

		.arg_R1 = 0xf0,
		.arg_G1 = 0x00,
		.arg_B1 = 0x00
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

	Command flash = {
		.mode = FLASH,
		.arg_R0 = 0x60,
		.arg_G0 = 0x60,
		.arg_B0 = 0xff,
		.arg_DT = 40,
		.arg_MOD = 3
	};

	/*execute(&gradient2);*/
	phase = 0;
	while(1) {
		/*execute(&rotate);*/
		execute(&flash);
		_delay_ms(1);

	}

	return 0;
}

void clear_leds() {
	memset(&led, 0, LEDS * sizeof(struct cRGB));
}

void execute(Command* c) {
	switch(c->mode) {
		case FULL:
			for(int i = 0; i < LEDS; i++) {
				led[i].r = c->arg_R0;
				led[i].g = c->arg_G0;
				led[i].b = c->arg_B0;
			}
			break;

		case MOD:
			clear_leds();
			for(int i = 0; i < LEDS / 2; i++) {
				if(i % c->arg_MOD == 0) {
				led[i].r = c->arg_R0;
				led[i].g = c->arg_G0;
				led[i].b = c->arg_B0;

				led[LEDS - i - 1].r = c->arg_R0;
				led[LEDS - i - 1].g = c->arg_G0;
				led[LEDS - i - 1].b = c->arg_B0;
				}
			}
			break;

		case GRADIENT:
			for(int i = 0; i < LEDS/2; i++) {
				led[i].r = c->arg_R0 + i * ((int)c->arg_R1 - (int)c->arg_R0) / (LEDS/2 - 1);
				led[i].g = c->arg_G0 + i * ((int)c->arg_G1 - (int)c->arg_G0) / (LEDS/2 - 1);
				led[i].b = c->arg_B0 + i * ((int)c->arg_B1 - (int)c->arg_B0) / (LEDS/2 - 1);
				led[LEDS - i - 1] = led[i];
			}
			break;

		case GRADIENT2:
			for(int i = 0; i < LEDS/4; i++) {
				led[i].r = c->arg_R0 + i * ((int)c->arg_R1 - (int)c->arg_R0) / (LEDS/4 - 1);
				led[i].g = c->arg_G0 + i * ((int)c->arg_G1 - (int)c->arg_G0) / (LEDS/4 - 1);
				led[i].b = c->arg_B0 + i * ((int)c->arg_B1 - (int)c->arg_B0) / (LEDS/4 - 1);
				led[LEDS/2 - i - 1] = led[i];
				led[LEDS/2 + i] = led[i];
				led[LEDS - i - 1] = led[i];
			}
			break;

		case ROTATE:
			if(milliseconds >= next_action_ms) {
				next_action_ms = milliseconds + c->arg_DT * 10;
				
				if(c->arg_MOD) {
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
				next_action_ms = milliseconds + c->arg_DT * 10;
				for(int i = 0; i < LEDS; i++) {
					if(led[i].r) { led[i].r = (led[i].r - 1) * 0.9; }
					if(led[i].g) { led[i].g = (led[i].g - 1) * 0.9; }
					if(led[i].b) { led[i].b = (led[i].b - 1) * 0.9; }
				}
			}
			break;

		case FLASH:
			if(milliseconds >= next_action_ms && phase/2 <= c->arg_MOD) {
				next_action_ms = milliseconds + c->arg_DT * 1;
				if(phase % 2 == 0) {
					for(int i = 0; i < LEDS; i++) {
						led[i].r = c->arg_R0;
						led[i].g = c->arg_G0;
						led[i].b = c->arg_B0;
					}
				}
				else {
					clear_leds();
				}
				phase++;
			}
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
	OCR1AL = CTC_MATCH_OVERFLOW;
 
	// Enable the compare match interrupt
	TIMSK1 |= (1 << OCIE1A);
	sei();
}

