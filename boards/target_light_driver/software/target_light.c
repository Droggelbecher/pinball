
#include <string.h> // memset & co

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <checksum.h>

#include "target_light.h"

#define ACTIVE (PINB & (1 << PB0))
#define DELAY 1000

int main(void) {
	setup_pins();

	while(1) {
		if(ACTIVE) {
			PORTC = (1 << PC1); _delay_ms(DELAY); PORTC = 0;
		}
		if(ACTIVE) {
			PORTC = (1 << PC2); _delay_ms(DELAY); PORTC = 0;
		}
		if(ACTIVE) {
			PORTC = (1 << PC3); _delay_ms(DELAY); PORTC = 0;
		}
	}

	return 0;
}

void setup_pins(void) {
	// outputs
	DDRC |= (1 << PC1) | (1 << PC2) | (1 << PC3);
	PORTC = 0;

	// PB0 = input for enable/disable, deactivate internal pullup
	PORTB &= ~(1 << PB0);
	DDRB &= ~(1 << PB0);
	PORTB &= ~(1 << PB0);
}

