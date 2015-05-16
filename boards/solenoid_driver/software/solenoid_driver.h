
#ifndef SOLENOID_DRIVER_H
#define SOLENOID_DRIVER_H

// Pins

#define FLIPPER_LEFT_POWER_PORT PORTC
#define FLIPPER_LEFT_POWER_DDR DDRC
#define FLIPPER_LEFT_POWER_PIN PINC0

#define FLIPPER_LEFT_HOLD_PORT PORTC
#define FLIPPER_LEFT_HOLD_DDR DDRC
#define FLIPPER_LEFT_HOLD_PIN PINC1

#define FLIPPER_RIGHT_POWER_PORT PORTC
#define FLIPPER_RIGHT_POWER_DDR DDRC
#define FLIPPER_RIGHT_POWER_PIN PINC2

#define FLIPPER_RIGHT_HOLD_PORT PORTC
#define FLIPPER_RIGHT_HOLD_DDR DDRC
#define FLIPPER_RIGHT_HOLD_PIN PINC3

#define DROP_TARGET_BANK_0_PORT PORTC
#define DROP_TARGET_BANK_0_DDR DDRC
#define DROP_TARGET_BANK_0_PIN PINC4


// how long do coils need to "cool down" (in 1/1024 secodns)?
#define FLIPPER_LEFT_COOLDOWN_TIME 1
#define FLIPPER_RIGHT_COOLDOWN_TIME 1
#define DROP_TARGET_BANK_0_COOLDOWN_TIME 100

// how long can coils be active before cooldown must start?
#define FLIPPER_LEFT_ACTIVE_TIME 10
#define FLIPPER_RIGHT_ACTIVE_TIME 10
#define DROP_TARGET_BANK_0_ACTIVE_TIME 10

#define FLIPPER_LEFT_CYCLE_TIME (FLIPPER_LEFT_COOLDOWN_TIME + FLIPPER_LEFT_ACTIVE_TIME)
#define FLIPPER_RIGHT_CYCLE_TIME (FLIPPER_RIGHT_COOLDOWN_TIME + FLIPPER_RIGHT_ACTIVE_TIME)
#define DROP_TARGET_BANK_0_CYCLE_TIME (DROP_TARGET_BANK_0_COOLDOWN_TIME + DROP_TARGET_BANK_0_ACTIVE_TIME)


#define BAUD 9600UL
// Berechnungen
#define UBRR_VAL ((F_CPU+BAUD*8)/(BAUD*16)-1)   // clever runden
#define BAUD_REAL (F_CPU/(16*(UBRR_VAL+1)))     // Reale Baudrate
#define BAUD_ERROR ((BAUD_REAL*1000)/BAUD) // Fehler in Promille, 1000 = kein Fehler.
#if ((BAUD_ERROR<990) || (BAUD_ERROR>1010))
  #error Systematischer Fehler der Baudrate grösser 1% und damit zu hoch! 
#endif


#include <unistd.h> // usleep
#include <util/delay.h>

#include <spi_protocols/solenoids.h>


// [0] = LSB (bits 0..7), [1] = MSB
// 1 -> solenoid active, 0 -> inactive
uint8_t solenoid_spi_state[2] = { 0xff, 0xff };
uint8_t solenoid_spi_state_idx = 0;

uint8_t cooldown_time[16];


int main(void);
void run_selftest(void);
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


#endif // SOLENOID_DRIVER_H

