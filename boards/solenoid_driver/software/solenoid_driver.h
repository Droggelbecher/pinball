
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

// SPI protocol indices

#define FLIPPER_LEFT_IDX   0
#define FLIPPER_RIGHT_IDX  1

// how long do coils need to "cool down" (in 1/1024 secodns)?
#define FLIPPER_LEFT_COOLDOWN_TIME 1
#define FLIPPER_RIGHT_COLLDOWN_TIME 1

// how long can coils be active before cooldown must start?
#define FLIPPER_LEFT_ACTIVE_TIME 10
#define FLIPPER_RIGHT_ACTIVE_TIME 10

#define FLIPPER_LEFT_CYCLE_TIME (FLIPPER_LEFT_COOLDOWN_TIME + FLIPPER_LEFT_ACTIVE_TIME)
#define FLIPPER_RIGHT_CYCLE_TIME (FLIPPER_RIGHT_COOLDOWN_TIME + FLIPPER_RIGHT_ACTIVE_TIME)


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

int main(void);
void run_selftest(void);
void run_main(void);
void setup_timer(void);
void setup_spi(void);
void setup_uart(void);
void uart_putc(char);
void uart_puts(char*);


#endif // SOLENOID_DRIVER_H

