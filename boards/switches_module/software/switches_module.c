
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


uint8_t spi_current_column = 0;
uint8_t spi_update = 0;

int main(void) {

	memset(debounce_state, 0, sizeof(debounce_state) * sizeof(debounce_state[0]));
	memset(debounce_alpha, 63, sizeof(debounce_alpha) * sizeof(debounce_alpha[0]));

	setup_switches();

	setup_spi();
	setup_uart();

	uart_puts("\aSWITCHES MODULE 0.1\r\n");

	/*DDRB |= (1 << PB0);*/
	/*uint8_t blink = 0;*/
	/*while(1) {*/
		/*PORTB ^= (1 << PB0);*/
		/*advance_sr(0);*/
		/*_delay_ms(1000);*/
		/*advance_sr(1);*/
		/*_delay_ms(1000);*/
	/*}*/

	
	while(1) {
		scan_switches();

		if(spi_update) {
			spi_update = 0;
			
			if(spi_current_column >= SWITCHES_COLUMNS) {
				spi_current_column = 0;
			}
			SPDR = switches_states[spi_current_column];
		}
	}
	
}

void setup_spi(void) {


	// MISO = output,
	DDRB |= (1 << PB4);

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
	SR_DDR = SR_DS | SR_STCP | SR_SHCP | SR_MR;

	DDRC &= ~0x1f; // PC0..5 = input for rows 0..5
	DDRB &= ~0x03; // PB0,1 = input for rows 6,7
}

/**
 * Postcondition:
 * Shift register and state register of the 74HC595 are
 * in a state where the first bit is HIGH and all other bits LOW.
 */
void reset_sr(void) {
	uint8_t oldstate = SR_PORT & ~(SR_DS | SR_STCP | SR_SHCP | SR_MR);

	// SR_MR low -> reset shift register
	/*SR_PORT = oldstate;*/
	// shift register -> output register (all low)
	/*SR_PORT = oldstate | SR_STCP;*/
	SR_PORT = oldstate;


	// 7 high bits

	int i;
	for(i = 0; i < 7; i++) {
		SR_PORT = oldstate | SR_DS | SR_MR ;
		SR_PORT = oldstate | SR_DS | SR_MR  | SR_SHCP;
		SR_PORT = oldstate | SR_DS | SR_MR ;
	}

	// ... and one low

	// load LOW into first register
	SR_PORT = oldstate | SR_MR ;
	SR_PORT = oldstate | SR_MR | SR_SHCP;
	SR_PORT = oldstate | SR_MR ;

	// load LOW into first register
	/*SR_PORT = oldstate | SR_MR ;*/
	/*SR_PORT = oldstate | SR_MR | SR_SHCP;*/
	/*SR_PORT = oldstate | SR_MR ;*/

	// shift register -> output register
	SR_PORT = oldstate | SR_MR;
	SR_PORT = oldstate | SR_MR | SR_STCP;
	/*SR_PORT = oldstate | SR_MR;*/
}


/**
 * Advance Shift register by one (SR <<= 1)
 */
void advance_sr(uint8_t colbit) {
	uint8_t oldstate = SR_PORT & ~(SR_DS | SR_STCP | SR_SHCP);
	
	uint8_t x = colbit ? SR_DS : 0;

	SR_PORT = oldstate | x;
	SR_PORT = oldstate | x | SR_SHCP;
	SR_PORT = oldstate | x;

	SR_PORT = oldstate | SR_STCP;

	/*

	SR_PORT = oldstate | x;
	SR_PORT = oldstate | x | SR_SHCP;
	SR_PORT = oldstate | x;

	// shift register -> output register
	SR_PORT |= SR_STCP;
	*/
}


void scan_switches(void) {
	int i;

	reset_sr();

	// rows 0...5 = PC0...PC5
	// PC0 | .. | PC5 = 0b11111 = 0x1f
	// rows 6,7 = PB0,PB1

	for(i = 0; i < 8; i++) {

		/*_delay_ms(100);*/

		uint8_t col_state = (PINC & 0x1f) | ((PINB & 0x03) << 6);
		int j;
		for(j = 0; j < 8; j++) {
			uint8_t bit = ((col_state >> j) & 1);

			debounce_state[i][j] =
				// bit*255 * alpha/255
				// = bit * alpha
				bit * debounce_alpha[i][j] + 

				// oldval * (1 - alpha/255)
				// ~= oldval * (255 - alpha)/256
				((uint16_t)debounce_state[i][j] * (uint16_t)(0xff - debounce_alpha[i][j])) / 256;
		}
		advance_sr(1);
	}

	for(i = 0; i < 8; i++) {
		switches_states[i] =
			(debounce_state[i][0] >= 0x80) << 0 |
			(debounce_state[i][1] >= 0x80) << 1 |
			(debounce_state[i][2] >= 0x80) << 2 |
			(debounce_state[i][3] >= 0x80) << 3 |
			(debounce_state[i][4] >= 0x80) << 4 |
			(debounce_state[i][5] >= 0x80) << 5 |
			(debounce_state[i][6] >= 0x80) << 6 |
			(debounce_state[i][7] >= 0x80) << 7;
		/*uart_puthex(switches_states[i]);*/
		uart_puthex(switches_states[i]);
		uart_putc(' ');
	}
	uart_puts("\r\n");
}





ISR(SPI_STC_vect) {
	spi_update = 1;
	spi_current_column = SPDR;
	/*char ch = SPDR;*/
	/*if(ch == C_EOT) {*/
		/*return;*/
	/*}*/
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

void uart_puthex(uint8_t x) {
	uart_putnibble(x >> 4);
	uart_putnibble(x & 0x0f);
}

void uart_putnibble(uint8_t x) {
	uart_putc(x < 10 ? '0' + x : 'a' + x - 10);
}



