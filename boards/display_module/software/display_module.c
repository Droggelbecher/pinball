
// SPI tutorial:
// http://www.engineersgarage.com/embedded/avr-microcontroller-projects/spi-serial-peripheral-interface-tutorial-circuit
//
//http://www.elecdude.com/2012/12/avr-spi-serial-pheripheral-interface.html
//
// UART:
//https://www.mikrocontroller.net/articles/AVR-GCC-Tutorial/Der_UART

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <string.h> // memset & co

#define ROW_DELAY 0.1
#define ROWS 16
#define COLUMNS 8
#define COLORS 2
#define RED 0
#define GREEN 1


#define HEARTBEAT 0


unsigned char screen[COLORS][ROWS][COLUMNS];
#define SCREEN_SIZE (COLORS * ROWS * COLUMNS)
unsigned char row = 0;
int demo_state = 1;
int demo_frame = 0;
unsigned char pwm_phase = 0;

volatile unsigned int screen_index = 0;


#define BAUD 9600UL      // Baudrate
//#define BAUD 38400UL      // Baudrate

// Berechnungen
#define UBRR_VAL ((F_CPU+BAUD*8)/(BAUD*16)-1)   // clever runden
#define BAUD_REAL (F_CPU/(16*(UBRR_VAL+1)))     // Reale Baudrate
#define BAUD_ERROR ((BAUD_REAL*1000)/BAUD) // Fehler in Promille, 1000 = kein Fehler.
#if ((BAUD_ERROR<990) || (BAUD_ERROR>1010))
  #error Systematischer Fehler der Baudrate grösser 1% und damit zu hoch! 
#endif

int main(void);
void setup_spi(void);
void setup_uart(void);
void setup_display(void);
void uart_putc(char);
void uart_puts(char*);
void output_screen(void);
void render_selftest(void);
void clear_screen(void);

int selftest = 1;


int main(void) {
	setup_spi();
	setup_uart();

	setup_display();
	clear_screen();
	output_screen();

	uart_puts("\aDISPLAY MODULE 0.1\r\n");

	PORTB |= (1 << PINB0);
	while(1) {
		
		/*if(!(PINB & (1 << PINB0))) {  //PB0/pin8 low --> self-test*/
			/*if(!selftest) {*/
				/*uart_puts("SELFTEST START\r\n");*/
			/*}*/
			/*selftest = 1;*/
			render_selftest();
		/*}*/
		/*else {*/
			/*if(selftest) {*/
				/*uart_puts("SELFTEST STOP\r\n");*/
			/*}*/
			/*selftest = 0;*/
		/*}*/
		
		long i = 0;
		for(i = 0 ; i < 512L; i++) {
			output_screen();
		}
	}
}

unsigned char V[] = { 0, 0x01, 0x03, 0x07, 0x0f, 31, 63, 127, 255 };
unsigned frame = 0;

void render_selftest(void) {
	int row = frame % ROWS;
	int color = frame / ROWS; // % (ROWS * COLORS)

	clear_screen();
	/*for(col = 0; col < COLS; col++) {*/
	memset(screen[color][row], V[7], COLUMNS);



	/*uart_putc('0' + frame);*/
	/*memset(screen, V[frame], SCREEN_SIZE);*/
	/*memset(screen, 0x01, SCREEN_SIZE);*/
	frame++;
	if(frame >= ROWS * COLORS) { frame = 0; }
}

void setup_spi(void) {
	// MISO = output, others input
	DDRB = 0x14;
	// Enable SPI, Master, set clock rate fck/16, SPI MODE 1
	SPCR = (1<<SPE)|(1<<SPIE); //|(1<<CPHA);

	// INT0 is wired to SS to inform us when a SPI transmission starts
	// so generate an interrupt on falling edge (SS low = transmission
	// running)
	/*MCUR = (1 << ISC01);*/
	/*GICR = (1 << INT0);*/



	// https://sites.google.com/site/qeewiki/books/avr-guide/external-interrupts-on-the-atmega328
	PCICR |= (1 << PCIE0);    // set PCIE0 to enable PCMSK0 scan
	// Generate interrupt on SS pin change
	PCMSK0 |= (1 << PCINT2);

	sei();
}

void setup_display(void) {
	// make port C output
	// (will be used to fill the shift registers)
	DDRC = 0xff;
}

void setup_uart(void) {
	UBRR0H = UBRR_VAL >> 8;
	UBRR0L = UBRR_VAL & 0xFF;

	UCSR0B |= (1<<TXEN0);                           // UART TX einschalten
	UCSR0A = (1 << UDRE0);
	UCSR0C = (1<<UCSZ01)|(1 << UCSZ00); // Asynchron 8N1
}

// SPI transmission start
ISR(PCINT0_vect) {
	if(PINB & (1 << PINB2)) {
		// low to high -> end of transmission
		//uart_puts("<end>\n");
		//uart_putc('>');
	}
	else {
		// high to low -> start of transmission
		screen_index = 0;
		//uart_puts("<start>\n");
		//uart_putc('<');
	}
}

ISR(SPI_STC_vect) {
	char ch = SPDR;
	if(selftest) {
		uart_putc('!');
		return;
	}
	//uart_putc('[');
	//uart_putc(ch);
	//uart_putc(']');
	//uart_putc('\n');
	if(screen_index >= SCREEN_SIZE) {
		screen_index = 0;
	}
	((unsigned char*)screen)[screen_index++] = (unsigned char)ch;
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

void shift(unsigned char x) {
	x = x ? 0b100 : 0b000;

	// SCK=1 SI=x
	PORTC = 0b001 | x;
	// SCK=0 SI=x
	PORTC = 0b000 | x;
}

void shift_row(void) {
	// the comments give the order of pins from QA...QH
	// note that we have to shift them in in the *reverse* order of that
	
	// green col 4 | row 4 | green col 5 | row 5 | ... | green col 7 | row 7
	for(int i = 7; i >= 4; i--) {
		shift(row == i);
		// what we name row is a column for the display (brainfuck!)
		shift(screen[GREEN][i + (row & 8)][row & 7] & pwm_phase);
	}
	
	// row 3 | green col 3 | ... | row 0 | green col 0
	for(int i = 0; i < 4; i++) {
		shift(screen[GREEN][i + (row & 8)][row & 7] & pwm_phase);
		shift(row == i);
	}
	
	// red col 4 | row 12 | ... | red col 7 | row 15
	for(int i = 7; i >= 4; i--) {
		shift(row == i + 8);
		shift(screen[RED][i + (row & 8)][row & 7] & pwm_phase);
	}
	
	// row 11 | red col 3 | ... | row 8 | red col 0
	for(int i = 0; i < 4; i++) {
		shift(screen[RED][i + (row & 8)][row & 7] & pwm_phase);
		shift(row == i + 8);
	}
}


void output_row(void) {
	if(row >= ROWS) { row = 0; }
	shift_row();
	
	// flank RCK --> write to status registers
	PORTC = 0b010;
	PORTC = 0b000;
	row++;
}

void output_blank_row(void) {
	if(row >= ROWS) { row = 0; }
	for(int i = 0; i < 32; i++) {
		shift(0);
	}
	// flank RCK --> write to status registers
	PORTC = 0b010;
	PORTC = 0b000;

	row++;
}

void output_screen(void) {
	for(int i = 0; i<16; i++) {
		output_row();
	}
	output_blank_row();
	pwm_phase <<= 1;
	if(pwm_phase == 0) {
		pwm_phase = 1;
	}
}

void clear_screen(void) {
	memset(screen, 0, SCREEN_SIZE);
}

