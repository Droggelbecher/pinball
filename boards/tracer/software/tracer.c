
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <string.h> // memset & co

#define BAUD 9600UL      // Baudrate
//#define BAUD 38400UL      // Baudrate

// Berechnungen
#define UBRR_VAL ((F_CPU+BAUD*8)/(BAUD*16)-1)   // clever runden
#define BAUD_REAL (F_CPU/(16*(UBRR_VAL+1)))     // Reale Baudrate
#define BAUD_ERROR ((BAUD_REAL*1000)/BAUD) // Fehler in Promille, 1000 = kein Fehler.
#if ((BAUD_ERROR<990) || (BAUD_ERROR>1010))
  #error Systematischer Fehler der Baudrate grösser 1% und damit zu hoch! 
#endif
void setup_spi(void);
void setup_uart(void);
void uart_puts(char*);
unsigned char uart_getc(void);
void uart_putc(char);
void output_buffer(void);
char hexnibble(unsigned char);

enum { C_EOT = 0xff };


/**
 * Simple tracer application for SPI.
 * To be run on an arduino in the role of an SPI slave.
 * Control via serial port.
 * 't' -> show the latest complete trace
 *        (trace end is defined by the symbol C_EOT = 0xff)
 */

enum { BUFFERS = 2,
	BUFFER_SIZE = 1024 };

unsigned char trace_buffer[BUFFERS][BUFFER_SIZE];
int buffer_pos[BUFFERS];
int active_buffer;
int do_trace = 1;

int main(void) {
	active_buffer = 0;
	buffer_pos[active_buffer] = 0;

	setup_spi();
	setup_uart();
	uart_puts("\aTRACER MODULE 0.1\r\n");
	while(1) {
		unsigned char c = uart_getc();
		if(c == 't') {
			output_buffer();
		}
	}
}

void output_buffer() {
	/*cli();*/
	do_trace = 0;

	int bufidx = active_buffer - 1;
	if(bufidx < 0) { bufidx += BUFFERS; }
	int len = buffer_pos[bufidx];
	if(len < 0 || len > BUFFER_SIZE) {
		len = BUFFER_SIZE;
	}
	for(int i = 0; i < len; ) {
		// print 8 chars in one row
		int j = i + 8;
		for( ; i < len && i < j; i++) {
			unsigned char ch = trace_buffer[bufidx][i];
			uart_putc(hexnibble(ch >> 4));
			uart_putc(hexnibble(ch & 0x0f));
			uart_putc(' ');
		}
		uart_putc('\r');
		uart_putc('\n');
	}
	uart_putc('\r');
	uart_putc('\n');

	/*sei();*/
	do_trace = 1;
}

char hexnibble(unsigned char x) {
	if(x >= 10) { return 'a' + (x - 10); }
	return '0' + x;
}

void setup_spi(void) {
	// MISO = output,
	// PB1 = output (SS for next module)
	// others input
	DDRB = PB4;
	// Enable SPI, Slave, set clock rate fck/16, SPI MODE 1
	// http://maxembedded.com/2013/11/the-spi-of-the-avr/
	SPCR = (1<<SPE)|(1<<SPIE); //|(1<<CPHA);
	sei();
}

void setup_uart(void) {
	UBRR0H = UBRR_VAL >> 8;
	UBRR0L = UBRR_VAL & 0xFF;

	UCSR0B |= (1<<TXEN0) | (1<<RXEN0);                           // UART TX einschalten
	UCSR0A = (1 << UDRE0);
	UCSR0C = (1<<UCSZ01)|(1 << UCSZ00); // Asynchron 8N1
}

ISR(SPI_STC_vect) {
	if(!do_trace) { return; }

	char ch = SPDR;
	if(buffer_pos[active_buffer] < BUFFER_SIZE) {
		trace_buffer[active_buffer][buffer_pos[active_buffer]++] = ch;
	}
	if(ch == C_EOT || buffer_pos[active_buffer] >= BUFFER_SIZE) {
		active_buffer++;
		if(active_buffer >= BUFFERS) {
			active_buffer = 0;
		}
		buffer_pos[active_buffer] = 0;
	}
}

/* Zeichen empfangen */
uint8_t uart_getc(void)
{
    while (!(UCSR0A & (1<<RXC0)))   // warten bis Zeichen verfuegbar
        ;
    return UDR0;                   // Zeichen aus UDR an Aufrufer zurueckgeben
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



