
/*#include <string.h> // memset & co*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>

/*#include <checksum.h>*/

#include "led_stripe.h"

#define LEDS 10
struct cRGB led[LEDS];

volatile unsigned long milliseconds;

// Command execution state
int current_led = 0;
unsigned long next_action_ms = 0;

int main(void) {
	setup();

	for(int i = 0; i < LEDS; i++) {
		led[i].r = led[i].g = led[i].b = 0;
	}
	ws2812_setleds(led, LEDS);

	Command yellow = {
		.mode = FULL,
		.arg_R0 = 0xff,
		.arg_G0 = 0xa0,
		.arg_B0 = 0x00
	};

	Command chaser = {
		.mode = CHASER,
		.arg_R0 = 0xff,
		.arg_G0 = 0xa0,
		.arg_B0 = 0x00,
		.arg_DT = 60, // *10ms
		.arg_MOD = 3 //LEDS / 2
	};

	Command fade = {
		.mode = FADE,
		.arg_R0 = 0xf0,
		.arg_G0 = 0xa0,
		.arg_B0 = 0x00,

		.arg_R1 = 0x10,
		.arg_G1 = 0x20,
		.arg_B1 = 0xa0
	};


	while(1) {
		execute(&fade);
		_delay_ms(1);

	}

	return 0;
}

void clear_leds() {
	/*memset(&led, 0, LEDS * sizeof(struct cRGB));*/
	for(int i = 0; i < LEDS; i++) {
		led[i].r = led[i].g = led[i].b = 0;
	}
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

		case CHASER:
			if(milliseconds >= next_action_ms) {
				next_action_ms = milliseconds + c->arg_DT * 10;
				current_led++;
				if(current_led >= LEDS / 2) {
					current_led = 0;
				}
			}

			clear_leds();

			for(int i = 0; i < LEDS / 2; i++) {
				if((i + current_led) % c->arg_MOD == 0) {
				led[i].r = c->arg_R0;
				led[i].g = c->arg_G0;
				led[i].b = c->arg_B0;

				led[LEDS - i - 1].r = c->arg_R0;
				led[LEDS - i - 1].g = c->arg_G0;
				led[LEDS - i - 1].b = c->arg_B0;
				}
			}

			break;

		case FADE:
			for(int i = 0; i < LEDS/2; i++) {
				led[i].r = c->arg_R0 + i * ((int)c->arg_R1 - (int)c->arg_R0) / (LEDS/2 - 1);
				led[i].g = c->arg_G0 + i * ((int)c->arg_G1 - (int)c->arg_G0) / (LEDS/2 - 1);
				led[i].b = c->arg_B0 + i * ((int)c->arg_B1 - (int)c->arg_B0) / (LEDS/2 - 1);
				led[LEDS - i - 1] = led[i];
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

