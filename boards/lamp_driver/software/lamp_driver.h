
#ifndef SOLENOID_DRIVER_H
#define SOLENOID_DRIVER_H

#define COLUMNS 8
#define ROWS 8

// shift register pins
#define SR_DDR DDRD
#define SR_PORT PORTD
// data input
#define SR_DS (1 << PD2)
// storage register clock
#define SR_STCP (1 << PD3)
// shift register clock
#define SR_SHCP (1 << PD4)
// reset shift register (active low)
//#define SR_MR (1 << PD5)


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


uint8_t lamp_state_buffer[2][ROWS];

uint8_t *lamp_states = lamp_state_buffer[0];
uint8_t *new_lamp_states = lamp_state_buffer[1];

uint8_t spi_xfer = 0;


int main(void);
void switch_lamp_state_buffers(void);
void mainloop(void);
void run_selftest(void);

inline void xfer_spi(void);

void setup_spi(void);
void setup_uart(void);
void setup_lamps(void);

void set_column(uint8_t);
void reset_sr(void);
void shift_one(void);
void shift_zero(void);

void uart_putc(char);
void uart_puts(char*);
void uart_puthex(uint8_t);
void uart_putnibble(uint8_t);

#endif // SOLENOID_DRIVER_H

