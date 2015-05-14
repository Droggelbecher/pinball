
// SPI tutorial:
// http://www.engineersgarage.com/embedded/avr-microcontroller-projects/spi-serial-peripheral-interface-tutorial-circuit
//
//http://www.elecdude.com/2012/12/avr-spi-serial-pheripheral-interface.html
//
// UART:
//https://www.mikrocontroller.net/articles/AVR-GCC-Tutorial/Der_UART

#include <avr/io.h>
#include <avr/interrupt.h>

#include <string.h> // memset & co

#include <checksum.h>
#include "solenoid_driver.h"

#define ENABLE_UART 1
#define SELFTEST    0


// [0] = LSB (bits 0..7), [1] = MSB
// 1 -> solenoid active, 0 -> inactive
uint8_t solenoid_spi_state[2];
uint8_t solenoid_spi_state_idx = 0;

uint8_t cooldown_time[16];


int main(void) {
	setup_spi();
	setup_uart();
	setup_timer();

	uart_puts("\aSOLENOID MODULE 0.1\r\n");

	// HIGH == Solenoid turned OFF

	FLIPPER_LEFT_POWER_DDR |= (1 << FLIPPER_LEFT_POWER_PIN);
	FLIPPER_LEFT_POWER_PORT |= (1 << FLIPPER_LEFT_POWER_PIN);

	FLIPPER_LEFT_HOLD_DDR |= (1 << FLIPPER_LEFT_HOLD_PIN);
	FLIPPER_LEFT_HOLD_PORT |= (1 << FLIPPER_LEFT_HOLD_PIN);

	FLIPPER_RIGHT_POWER_DDR |= (1 << FLIPPER_RIGHT_POWER_PIN);
	FLIPPER_RIGHT_POWER_PORT |= (1 << FLIPPER_RIGHT_POWER_PIN);

	FLIPPER_RIGHT_HOLD_DDR |= (1 << FLIPPER_RIGHT_HOLD_PIN);
	FLIPPER_RIGHT_HOLD_PORT |= (1 << FLIPPER_RIGHT_HOLD_PIN);

	cooldown_time[FLIPPER_LEFT_IDX] = 0;
	cooldown_time[FLIPPER_RIGHT_IDX] = 0;


#if SELFTEST
		run_selftest();
#else
		run_main();
#endif
	
}

void run_main(void) {
	while(1) {

		// if (
		// 		activity requested &&
		// 		not yet active &&
		// 		cooled down &&
		// 		 /*not active too long*/
		// ) {
		// 		set active
		// 		set to not cooled down
		// 		start activity timeout
		// }
		//
		//
		// if (
		// 		activity requested &&
		// 		not yet hot &&
		// 		 /*not active too long*/
		// ) {
		// 		keep power coil active
		// }
		//
		// if (
		// 		activity requested &&
		// 		active before &&
		// 		cooling down
		// ) {
		// 		use hold coil
		// }
		//

		uint8_t fl_left_active_before = (FLIPPER_LEFT_POWER_PORT & ((1 << FLIPPER_LEFT_POWER_PIN) | (1 << FLIPPER_LEFT_HOLD_PIN))) != ((1 << FLIPPER_LEFT_POWER_PIN) | (1 << FLIPPER_LEFT_HOLD_PIN));
		uint8_t fl_right_active_before = (FLIPPER_RIGHT_POWER_PORT & ((1 << FLIPPER_RIGHT_POWER_PIN) | (1 << FLIPPER_RIGHT_HOLD_PIN))) != ((1 << FLIPPER_RIGHT_POWER_PIN) | (1 << FLIPPER_RIGHT_HOLD_PIN));


		uint8_t fl_left_power = 0,
				fl_left_hold = 0,
				fl_right_power = 0,
				fl_right_hold = 0;

		/*uart_puthex(solenoid_spi_state[0]);*/
		/*uart_puthex(solenoid_spi_state[1]);*/
		/*uart_puts("\r\n");*/

		// activity of this solenoid requested
		if(!(solenoid_spi_state[FLIPPER_LEFT_IDX / 8] & (1 << (FLIPPER_LEFT_IDX % 8)))) {
			if(
					!fl_left_active_before &&
					(cooldown_time[FLIPPER_LEFT_IDX] == 0)
			) {
				fl_left_power = 1;
				cooldown_time[FLIPPER_LEFT_IDX] = FLIPPER_LEFT_CYCLE_TIME;
			}

			else if(cooldown_time[FLIPPER_LEFT_IDX] > FLIPPER_LEFT_COOLDOWN_TIME) {
				fl_left_power = 1;
			}

			else if(fl_left_active_before) {
				fl_left_hold = 1;
			}
		}

		if(!(solenoid_spi_state[FLIPPER_RIGHT_IDX / 8] & (1 << (FLIPPER_RIGHT_IDX % 8)))) {
			if(
					!fl_right_active_before &&
					(cooldown_time[FLIPPER_RIGHT_IDX] == 0)
			) {
				fl_right_power = 1;
				cooldown_time[FLIPPER_RIGHT_IDX] = FLIPPER_RIGHT_CYCLE_TIME;
			}

			else if(cooldown_time[FLIPPER_RIGHT_IDX] > FLIPPER_RIGHT_COOLDOWN_TIME) {
				fl_right_power = 1;
			}

			else if(fl_right_active_before) {
				fl_right_hold = 1;
			}
		}

		if(fl_left_power) { FLIPPER_LEFT_POWER_PORT &= ~(1 << FLIPPER_LEFT_POWER_PIN); }
		else { FLIPPER_LEFT_POWER_PORT |= (1 << FLIPPER_LEFT_POWER_PIN); }
		if(fl_left_hold) { FLIPPER_LEFT_HOLD_PORT &= ~(1 << FLIPPER_LEFT_HOLD_PIN); }
		else { FLIPPER_LEFT_HOLD_PORT |= (1 << FLIPPER_LEFT_HOLD_PIN); }

		if(fl_right_power) { FLIPPER_RIGHT_POWER_PORT &= ~(1 << FLIPPER_RIGHT_POWER_PIN); }
		else { FLIPPER_RIGHT_POWER_PORT |= (1 << FLIPPER_RIGHT_POWER_PIN); }
		if(fl_right_hold) { FLIPPER_RIGHT_HOLD_PORT &= ~(1 << FLIPPER_RIGHT_HOLD_PIN); }
		else { FLIPPER_RIGHT_HOLD_PORT |= (1 << FLIPPER_RIGHT_HOLD_PIN); }
	}
}

void run_selftest(void) {
	while(1) {
		/*usleep(5UL * 1000000UL);*/
		_delay_ms(5000);

		// Power coil on

		FLIPPER_LEFT_POWER_PORT &= ~(1 << FLIPPER_LEFT_POWER_PIN);
		_delay_ms(10);

		// Power coil off
		// Hold coil on

		FLIPPER_LEFT_POWER_PORT |= (1 << FLIPPER_LEFT_POWER_PIN);
		
		/*_delay_ms(100);*/

		FLIPPER_LEFT_HOLD_PORT &= ~(1 << FLIPPER_LEFT_HOLD_PIN);

		_delay_ms(1000);

		// Hold coil off

		FLIPPER_LEFT_HOLD_PORT |= (1 << FLIPPER_LEFT_HOLD_PIN);
	}
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

	TIMSK0 |= (1 << OCIE0A);    //Set the ISR COMPA vect

	sei();

	// start with prescaler = 1024 -> @16MHz that means 16k ticks / second
	TCCR0B |= (1 << CS00) | (1 << CS02);
}

void setup_spi(void) {
	// MISO = output,
	// PB1 = output (SS for next module)
	// others input
	DDRB = (1<<PB1) | (1<<PB4);
	// Enable SPI, Slave, set clock rate fck/16, SPI MODE 1
	// http://maxembedded.com/2013/11/the-spi-of-the-avr/
	SPCR = (1<<SPE)|(1<<SPIE); //|(1<<CPHA);


	// https://sites.google.com/site/qeewiki/books/avr-guide/external-interrupts-on-the-atmega328
	PCICR |= (1 << PCIE0);    // set PCIE0 to enable PCMSK0 scan
	// Generate interrupt on SS pin change
	PCMSK0 |= (1 << PCINT2);

	sei();
}

void setup_uart(void) {
	UBRR0H = UBRR_VAL >> 8;
	UBRR0L = UBRR_VAL & 0xFF;

	UCSR0B |= (1<<TXEN0);                           // UART TX einschalten
	UCSR0A = (1 << UDRE0);
	UCSR0C = (1<<UCSZ01)|(1 << UCSZ00); // Asynchron 8N1
}

ISR(TIMER0_COMPA_vect) {
	int i = 0;
	for(i = 0; i < sizeof(cooldown_time); i++) {
		if(cooldown_time[i] > 0) { cooldown_time[i]--; }
	}
}

ISR(PCINT0_vect) {
	solenoid_spi_state_idx = 0;
	/*uart_putc('|');*/
}

ISR(SPI_STC_vect) {
	static uint8_t spi_buf[] = { 0xff, 0xff, 0xff };

	char ch = SPDR;

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
	while (!(UCSR0A & (1<<UDRE0)))  /* warten bis Senden moeglich                   */
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



