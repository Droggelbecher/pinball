
#ifndef LINE_DEBUGGER_H
#define LINE_DEBUGGER_H

#define BAUD 56000UL
// Berechnungen
#define UBRR_VAL ((F_CPU+BAUD*8)/(BAUD*16)-1)   // clever runden
#define BAUD_REAL (F_CPU/(16*(UBRR_VAL+1)))     // Reale Baudrate
#define BAUD_ERROR ((BAUD_REAL*1000)/BAUD) // Fehler in Promille, 1000 = kein Fehler.
#if ((BAUD_ERROR<990) || (BAUD_ERROR>1010))
  #error Systematischer Fehler der Baudrate grösser 1% und damit zu hoch! 
#endif

int main(void);
void setup_uart(void);
void setup_adc(void);
void uart_putc(char);
void uart_puts(char*);
void uart_puthex16(uint16_t);
void uart_puthex(uint8_t);
void uart_putnibble(uint8_t);



#endif // LINE_DEBUGGER_H

