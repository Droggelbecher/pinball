
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

#include "solenoid_driver.h"


int selftest = 1;

int main(void) {
	setup_spi();
	setup_uart();

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


	if(selftest) {
		run_selftest();
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


void setup_spi(void) {
	// MISO = output,
	// PB1 = output (SS for next module)
	// others input
	DDRB = PB1 | PB4;
	// Enable SPI, Slave, set clock rate fck/16, SPI MODE 1
	// http://maxembedded.com/2013/11/the-spi-of-the-avr/
	SPCR = (1<<SPE)|(1<<SPIE); //|(1<<CPHA);

	// INT0 is wired to SS to inform us when a SPI transmission starts
	// so generate an interrupt on falling edge (SS low = transmission
	// running)
	/*MCUR = (1 << ISC01);*/
	/*GICR = (1 << INT0);*/



	// https://sites.google.com/site/qeewiki/books/avr-guide/external-interrupts-on-the-atmega328
	/*PCICR |= (1 << PCIE0);    // set PCIE0 to enable PCMSK0 scan*/
	// Generate interrupt on SS pin change
	/*PCMSK0 |= (1 << PCINT2);*/

	sei();
}

void setup_uart(void) {
	UBRR0H = UBRR_VAL >> 8;
	UBRR0L = UBRR_VAL & 0xFF;

	UCSR0B |= (1<<TXEN0);                           // UART TX einschalten
	UCSR0A = (1 << UDRE0);
	UCSR0C = (1<<UCSZ01)|(1 << UCSZ00); // Asynchron 8N1
}

ISR(SPI_STC_vect) {
	char ch = SPDR;
	if(ch == C_EOT) {
		return;
	}
}

void uart_putc(char x) {
	// bei neueren AVRs steht der Status in UCSRA/UCSR0A/UCSR1A, hier z.B. fuer ATmega16:
	while (!(UCSR0A & (1<<UDRE0)))  /* warten bis Senden moeglich                   */
	{
	}
	UDR0 = x;
}

void uart_puts(char *s) {
	while(*s) { uart_putc(*(s++)); }
}



