
/*#include <string.h> // memset & co*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/*#include <checksum.h>*/

#include "led_stripe.h"

#define LEDS 6
struct cRGB led[LEDS];


int main(void) {
	setup_pins();

	for(int i = 0; i < LEDS; i++) {
		led[i].r = led[i].g = led[i].b = 0;
	}

	while(1) {
		led[0].r++;
		led[1].g++;
		led[2].b++;

		led[3].r++; led[3].g++;
		led[4].g++; led[4].b++;
		led[5].b++; led[5].r++;

		ws2812_setleds(led, LEDS);
		_delay_ms(100);

	}

	return 0;
}

void setup_pins(void) {
	/*DDRF |= (1 << PF0);*/
	DDRC |= (1 << PC0);
}

