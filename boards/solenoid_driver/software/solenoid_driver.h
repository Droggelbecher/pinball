
#ifndef SOLENOID_DRIVER_H
#define SOLENOID_DRIVER_H

#define BAUD 9600UL
#define UBRR_VAL ((F_CPU+BAUD*8)/(BAUD*16)-1)   // clever runden
#define BAUD_REAL (F_CPU/(16*(UBRR_VAL+1)))     // Reale Baudrate
#define BAUD_ERROR ((BAUD_REAL*1000)/BAUD) // Fehler in Promille, 1000 = kein Fehler.
#if ((BAUD_ERROR<990) || (BAUD_ERROR>1010))
  #error Systematischer Fehler der Baudrate grösser 1% und damit zu hoch! 
#endif


#include <unistd.h> // usleep
#include <util/delay.h>

#include <spi_protocols/solenoids.h>

typedef struct {
	volatile uint8_t *port;
	volatile uint8_t *ddr;
	uint8_t pin;
	uint8_t cooldown;
	uint8_t active;
	uint8_t t;
} Solenoid;

Solenoid solenoids[];


// [0] = LSB (bits 0..7), [1] = MSB
// 1 -> solenoid active, 0 -> inactive
uint8_t solenoid_spi_state[2] = { 0xff, 0xff };
uint8_t solenoid_spi_state_idx = 0;

//uint8_t cooldown_time[16];

uint8_t in_selftest = 0;
uint16_t selftest_delay = 0;
uint8_t selftest_pos = 0;

int main(void);
void toggle_selftest(void);
void run_main(void);
void setup_timer(void);
void setup_spi(void);
void setup_uart(void);
void uart_putc(char);
void uart_puts(char*);
void uart_puthex(uint8_t);
void uart_putnibble(uint8_t);

/**
 * Return the state of a solenoid as requested via SPI (1 -> active).
 */ 
uint8_t get_state(uint8_t idx);

inline uint8_t get_state(uint8_t idx) {
	return !(solenoid_spi_state[idx / 8] & (1 << (idx % 8)));
}

void clear_state(void);

inline void clear_state() {
	solenoid_spi_state[0] = 0xff;
	solenoid_spi_state[1] = 0xff;
}

inline void set_state(uint8_t idx) {
	solenoid_spi_state[idx / 8] &= (0xff - (1 << (idx % 8)));
}

#endif // SOLENOID_DRIVER_H

