
// SPI tutorial:
// http://www.engineersgarage.com/embedded/avr-microcontroller-projects/spi-serial-peripheral-interface-tutorial-circuit
// 
//http://www.elecdude.com/2012/12/avr-spi-serial-pheripheral-interface.html
//
// UART: //https://www.mikrocontroller.net/articles/AVR-GCC-Tutorial/Der_UART

#include <avr/io.h>
#include <avr/interrupt.h>

#include <string.h> // memset & co

#include <checksum.h>
#include "solenoid_driver.h"

#define ENABLE_UART 0

Solenoid solenoids[] = {
	// port,  ddr,   pin, cooldown, active, t
	
	// Flipper left power
	{ &PORTC, &DDRC, PINC0, 1, 10, 0 },
	// Flipper left hold
	{ &PORTC, &DDRC, PINC1, 1, 255, 0 },
	// Flipper right power
	{ &PORTC, &DDRC, PINC2, 1, 10, 0 },
	// Flipper right hold
	{ &PORTC, &DDRC, PINC3, 1, 255, 0 },

	// Drop Target Bank 0
	{ &PORTC, &DDRC, PINC4, 100, 100, 0 },

	// Slingshot left
	{ &PORTC, &DDRC, PINC5, 100, 100, 0 },

	// Slingshot right
	{ &PORTB, &DDRB, PINB1, 100, 100, 0 },

	// Bumper 0
	{ &PORTD, &DDRD, PIND7, 100, 100, 0 },
	
	// Bumper 1
	{ &PORTD, &DDRD, PIND6, 100, 100, 0 },

	// Bumper 2
	{ &PORTD, &DDRD, PIND5, 100, 100, 0 },

	// Aux 0 (transistor broken?)
	{ &PORTD, &DDRD, PIND2, 100, 100, 0 },

	// Ball return
	{ &PORTD, &DDRD, PIND3, 500, 200, 0 },

	// Aux 1
	{ &PORTD, &DDRD, PIND4, 100, 100, 0 }

};

enum {
	SN_FLIPPER_LEFT_POWER = 0,
	SN_FLIPPER_LEFT_HOLD,
	SN_FLIPPER_RIGHT_POWER,
	SN_FLIPPER_RIGHT_HOLD,
	SN_DROP_TARGET_BANK_0,
	SN_SLINGSHOT_LEFT,
	SN_SLINGSHOT_RIGHT,
	SN_BUMPER_0,
	SN_BUMPER_1,
	SN_BUMPER_2,
	SN_AUX_0,
	SN_BALL_RETURN,
	SN_AUX_1
};




uint8_t is_pin_active(Solenoid *s) {
	return (*(s->port) & (1 << s->pin)) == 0;
}

uint8_t is_state_cool(Solenoid *s) {
	return s->t == 0;
}

uint8_t is_state_active(Solenoid *s) {
	return s->t > s->cooldown;
}

void start_active_state(Solenoid *s) {
	s->t = s->cooldown + s->active;
}

void check_flipper(int state_index_power, int state_index_hold, int spi_index) {

	uint8_t power = 0;
	uint8_t hold = 0;

	Solenoid *s_power = &solenoids[state_index_power];
	Solenoid *s_hold = &solenoids[state_index_hold];

	uint8_t active_before = is_pin_active(s_power)
		|| is_pin_active(s_hold);

	if(get_state(spi_index)) {
		if(!active_before && is_state_cool(s_power)) {
			start_active_state(s_power);
			power = 1;
		}

		else if(is_state_active(s_power)) {
			power = 1;
		}

		else if(active_before) {
			hold = 1;
		}
	}

	if(power) {
		*(s_power->port) &= ~(1 << s_power->pin);
	}
	else {
		*(s_power->port) |= 1 << s_power->pin;
	}
	
	if(hold) {
		*(s_hold->port) &= ~(1 << s_hold->pin);
	}
	else {
		*(s_hold->port) |= 1 << s_hold->pin;
	}
}


void check_solenoid(int state_index, int spi_index) {

	uint8_t power = 0;

	Solenoid *s = &solenoids[state_index];

	uint8_t active_before = is_pin_active(s);

	if(get_state(spi_index)) {
		if(!active_before && is_state_cool(s)) {
			start_active_state(s);
			power = 1;
		}

		else if(is_state_active(s)) {
			power = 1;
		}
	}

	if(power) {
		*(s->port) &= ~(1 << s->pin);
	}
	else {
		*(s->port) |= 1 << s->pin;
	}
}




int main(void) {
	for(int i = 0; i < sizeof(solenoids) / sizeof(solenoids[0]); i++) {
		*(solenoids[i].ddr) |= (1 << solenoids[i].pin);
		*(solenoids[i].port) |= (1 << solenoids[i].pin);
		solenoids[i].t = 0;
	}
	
	setup_spi();
	setup_uart();
	setup_timer();

	uart_puts("\aSOLENOID MODULE 0.1\r\n");

	// Make all solenoids output and turn all solenoids off (high)
	


	// PB0 low --> selftest
	PORTB |= (1 << PB0);
	DDRB &= ~(1 << PB0);
	PORTB |= (1 << PB0);

	/*memset(cooldown_time, 0, sizeof(cooldown_time));*/

	run_main();
	
}

void check_selftest(void);

void run_main(void) {
	while(1) {
		check_selftest();

		// Special treatment for flipper double-coils

		check_flipper(SN_FLIPPER_LEFT_POWER, SN_FLIPPER_LEFT_HOLD, SPI_SOLENOIDS_FLIPPER_LEFT_IDX);
		check_flipper(SN_FLIPPER_RIGHT_POWER, SN_FLIPPER_RIGHT_HOLD, SPI_SOLENOIDS_FLIPPER_RIGHT_IDX);

		check_solenoid(SN_DROP_TARGET_BANK_0, SPI_SOLENOIDS_DROP_TARGET_BANK_0_IDX);
		check_solenoid(SN_SLINGSHOT_LEFT, SPI_SOLENOIDS_SLINGSHOT_LEFT_IDX);
		check_solenoid(SN_SLINGSHOT_RIGHT, SPI_SOLENOIDS_SLINGSHOT_RIGHT_IDX);
		check_solenoid(SN_BUMPER_0, SPI_SOLENOIDS_BUMPER_0_IDX);
		check_solenoid(SN_BUMPER_1, SPI_SOLENOIDS_BUMPER_1_IDX);
		check_solenoid(SN_BUMPER_2, SPI_SOLENOIDS_BUMPER_2_IDX);
		check_solenoid(SN_BALL_RETURN, SPI_SOLENOIDS_BALL_RETURN_IDX);
		check_solenoid(SN_AUX_0, SPI_SOLENOIDS_AUX_0_IDX);
		check_solenoid(SN_AUX_1, SPI_SOLENOIDS_AUX_1_IDX);

	} // while
}

void check_selftest(void) {
	//if(selftest_delay <= 10) {
		in_selftest = ((PINB & (1 << PB0)) == 0);
	//}
	if(in_selftest) {
		clear_state();
		set_state(selftest_pos);
	}
}

void toggle_selftest(void) {

}

void setup_timer(void) {
	// source: https://www.mikrocontroller.net/articles/AVR-GCC-Tutorial/Die_Timer_und_Z%C3%A4hler_des_AVR
	// source: https://sites.google.com/site/qeewiki/books/avr-guide/timers-on-the-atmega328
	//

	// mode = CTC
	TCCR0A |= (1 << WGM01);

	// count to 16 -> reached 1024 times / second (that is after little more
	// that 1ms)
	OCR0A = 16;

	TIMSK0 |= (1 << OCIE0A); //Set the ISR COMPA vect

	sei();

	// start with prescaler = 1024 -> @16MHz that means 16k ticks / second
	TCCR0B |= (1 << CS00) | (1 << CS02);
}

void setup_spi(void) {
	// MISO = output,
	// PB1, PB4 = output
	// others input
	DDRB = (1<<PB1) | (1<<PB4);
	// Enable SPI, Slave, set clock rate fck/16, SPI MODE 1
	// http://maxembedded.com/2013/11/the-spi-of-the-avr/
	SPCR = (1<<SPE)|(1<<SPIE); //|(1<<CPHA);


	// https://sites.google.com/site/qeewiki/books/avr-guide/external-interrupts-on-the-atmega328
	PCICR |= (1 << PCIE0); // set PCIE0 to enable PCMSK0 scan
	// Generate interrupt on SS pin change
	PCMSK0 |= (1 << PCINT2);

	sei();
}

void setup_uart(void) {
	UBRR0H = UBRR_VAL >> 8;
	UBRR0L = UBRR_VAL & 0xFF;

	UCSR0B |= (1<<TXEN0); // UART TX einschalten
	UCSR0A = (1 << UDRE0);
	UCSR0C = (1<<UCSZ01)|(1 << UCSZ00); // Asynchron 8N1
}

ISR(TIMER0_COMPA_vect) {
	for(int i = 0; i < sizeof(solenoids) / sizeof(solenoids[0]); i++) {
		if(solenoids[i].t > 0) {
			--(solenoids[i].t);
		}
	}

	if(selftest_delay > 0) {
		--selftest_delay;
	}
	else {
		selftest_delay = 1000;
		++selftest_pos;
		if(selftest_pos >= 16) {
			selftest_pos = 0;
		}
	}
}

ISR(PCINT0_vect) {
	solenoid_spi_state_idx = 0;
	/*uart_putc('|');*/
}

ISR(SPI_STC_vect) {
	static uint8_t spi_buf[] = { 0xff, 0xff, 0xff };

	char ch = SPDR;

	if(in_selftest) { return; }

	/*uart_putc('s');*/

	if(solenoid_spi_state_idx > 2) { return; }

	spi_buf[solenoid_spi_state_idx] = ch;
	if(solenoid_spi_state_idx == 2) {
		if(checksum(spi_buf, 2) == spi_buf[2]) {
			solenoid_spi_state[0] = spi_buf[0];
			solenoid_spi_state[1] = spi_buf[1];
			/*uart_puts(":-)\r\n");*/
		}
		else {
			uart_putc('!');
			uart_puthex(checksum(spi_buf, 2));
			uart_putc(' ');
			uart_puthex(spi_buf[2]);
			uart_puts("\r\n");
		}
	}

	++solenoid_spi_state_idx;
}

void uart_putc(char x) {
#if ENABLE_UART
	// bei neueren AVRs steht der Status in UCSRA/UCSR0A/UCSR1A, hier z.B. fuer ATmega16:
	while (!(UCSR0A & (1<<UDRE0))) /* warten bis Senden moeglich */
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



