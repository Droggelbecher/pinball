
#ifndef SOLENOID_DRIVER_H
#define SOLENOID_DRIVER_H


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

enum {

	C_EOT = 0xff
};

int main(void);
void run_selftest(void);
void setup_spi(void);
void setup_uart(void);
void uart_putc(char);
void uart_puts(char*);


#endif // SOLENOID_DRIVER_H

