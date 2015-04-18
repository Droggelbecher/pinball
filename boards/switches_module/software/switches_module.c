
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

#include "switches_module.h"


int selftest = 1;

int main(void) {
	setup_switches();

	setup_spi();
	setup_uart();

	uart_puts("\aSWITCHES MODULE 0.1\r\n");
}

void setup_spi(void) {


	// MISO = output,
	DDRB |= PB4;

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

void setup_switches(void) {
	// PD2..PD4 output (column control via shift register)
	// PD5, PD6 input (rows 6,7)
	/*DDRD = PD2 | PD3 | PD4;*/
	SR_DDR = SR_DS | SR_STCP | SR_SHCP;

	DDRC &= ~0x1f; // PC0..5 = input for rows 0..5
	DDRB &= ~0x03; // PB0,1 = input for rows 6,7
}

/**
 * Postcondition:
 * Shift register and state register of the 74HC595 are
 * in a state where the first bit is HIGH and all other bits LOW.
 */
void reset_sr(void) {
	uint8_t oldstate = SR_PORT & ~(SR_SHCP | SR_STCP | SR_SHCP | SR_MR);

	// SR_MR low -> reset shift register
	SR_PORT |= oldstate;
	// shift register -> output register (all low)
	SR_PORT |= oldstate | SR_STCP;

	// load HIGH into first register
	SR_PORT |= oldstate | SR_MR | SR_DS;
	SR_PORT |= oldstate | SR_MR | SR_DS | SR_SHCP;

	SR_PORT |= oldstate | SR_MR;
	SR_PORT |= oldstate | SR_MR | SR_STCP;
}


/**
 * Advance Shift register by one (SR <<= 1)
 */
void advance_sr(bool colbit) {
	uint8_t oldstate = SR_PORT & ~(SR_SHCP | SR_STCP | SR_SHCP);
	SR_PORT = oldstate | SHCP | colbit;
	SR_PORT = oldstate | colbit;
}


void scan_switches(void) {
	int i;
	uint8_t switch_states;

	reset_sr();

	// rows 0...5 = PC0...PC5
	// PC0 | .. | PC5 = 0b11111 = 0x1f
	// rows 6,7 = PB0,PB1

	for(i = 0; i < 7; i++) {
		switches_states[i] = (PINC & 0x1f) | ((PINB & 0x03) << 6);
		advance_sr();
	}
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



