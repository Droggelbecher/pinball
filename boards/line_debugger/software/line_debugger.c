
#include <avr/sleep.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h> // memset & co
#include <stdint.h>
#include <util/delay.h>

#include "line_debugger.h"

#define SAMPLE_START_PIN 1
#define SAMPLE_PINS 4
#define SAMPLES 16

volatile uint16_t samples[SAMPLE_PINS];
volatile uint8_t pin_idx = 0;
volatile uint8_t adc_done = 0;
volatile uint8_t sample_idx = 0;

int main(void) {
	int i;
	
	setup_uart();
	setup_adc();

	DDRB |= (1 << PB5);
	PORTB &= ~(1 << PB5);

	uart_puts("LINE DEBUGGER v0.1\r\n");

	uint8_t started = 0;
	while(1) {
		/*uart_putc(adc_done ? 'X' : 'x');*/
		/*uart_putc(started ? 'Y' : 'y');*/
		/*uart_putc('a' + pin_idx);*/
		/*uart_putc('A' + sample_idx);*/

		if(adc_done) {
			// division for averaging
			for(i = 0; i< SAMPLE_PINS; i++) {
				/*samples[i] /= SAMPLES;*/

				uart_puthex16(samples[i]);
				uart_putc(' ');
			}
			uart_puts("\r\n");

			memset((uint8_t*)samples, 0x00, sizeof(samples));

			_delay_ms(5);
			adc_done = 0;
			started = 0;
		}
		
		if(!started) {
			started = 1;
			// ADSC=start conversion, this bit will drop to 0 when done
			// ADIE=enable interrupt on measurement complete
			// ADEN=enable adc
			ADCSRA = _BV(ADIE) | _BV(ADEN) | _BV(ADPS0) | _BV(ADPS2);
			sei();

			/*set_sleep_mode(SLEEP_MODE_ADC);*/
			ADCSRA |= _BV(ADSC);
			/*sleep_mode();*/
		}

		/*_delay_ms(100);*/
		/*else if(started) {*/
			/*set_sleep_mode(SLEEP_MODE_ADC);*/
			/*sleep_mode();*/
		/*}*/
	}
	return 0;
}

void setup_uart(void) {
	UBRR0H = UBRR_VAL >> 8;
	UBRR0L = UBRR_VAL & 0xFF;

	UCSR0B |= (1<<TXEN0);                           // UART TX einschalten
	UCSR0A = (1 << UDRE0);
	UCSR0C = (1<<UCSZ01)|(1 << UCSZ00); // Asynchron 8N1
}

void setup_adc(void) {
	// source: https://www.mikrocontroller.net/articles/AVR-GCC-Tutorial/Analoge_Ein-_und_Ausgabe

	// set the analog reference (high two bits of ADMUX) and select the
	// channel (low 4 bits).  this also sets ADLAR (left-adjust result)
	// to 0 (the default).
	// 
	// REFS1 REFS0
	// 0		0		external AREF
	// 0		1		AVcc
	// 1		0		reserved
	// 1		1		internal 2,56V
	// 
	// 
	ADMUX = /*_BV(REFS1) |*/ _BV(REFS0) | (SAMPLE_START_PIN & 0x07);

	// ACIE = enable interrupt
	//
	// ACIS1 ACIS0
	// 0     0      
	// 0     1
	// 1     0
	// 1     1
	ACSR = _BV(ACD); // disable comparator

	memset((uint8_t*)samples, 0x00, sizeof(samples));
	sei();
}

ISR(ADC_vect) {
	// trigged when a reading is available
	pin_idx = (ADMUX & 0x07) - SAMPLE_START_PIN;
	//low = ADCL;
	//high = ADCH;
	samples[pin_idx] = ADCW; //(high << 8) | low;
	pin_idx++;
	
	if(pin_idx >= SAMPLE_PINS) {
		pin_idx = 0;
		sample_idx++;
		if(sample_idx >= SAMPLES) {
			ADCSRA &= ~_BV( ADIE );  // turn off ADC interrupt
		
			sample_idx = 0;
			adc_done = 1;
			return;
		}
	}

	ADMUX = _BV(REFS0) | ((pin_idx + SAMPLE_START_PIN) & 0x07);

	/*ADCSRA |= _BV(ADIE) | _BV(ADEN);*/
	/*ADCSRA = _BV(ADIE) | _BV(ADEN) | _BV(ADPS0) | _BV(ADPS2);*/
	/*sei();*/
	/*set_sleep_mode(SLEEP_MODE_ADC);*/

	ADCSRA |= _BV(ADSC);
	/*sleep_mode();*/
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

void uart_puthex16(uint16_t x) {
	/*uart_putnibble(x >> 28);*/
	/*uart_putnibble((x >> 24) & 0x0f);*/
	/*uart_putnibble((x >> 20) & 0x0f);*/
	/*uart_putnibble((x >> 16) & 0x0f);*/
	uart_putnibble((x >> 12) & 0x0f);
	uart_putnibble((x >> 8) & 0x0f);
	uart_putnibble((x >> 4) & 0x0f);
	uart_putnibble(x & 0x0f);
}

void uart_puthex(uint8_t x) {
	uart_putnibble(x >> 4);
	uart_putnibble(x & 0x0f);
}

void uart_putnibble(uint8_t x) {
	uart_putc(x < 10 ? '0' + x : 'a' + x - 10);
}

