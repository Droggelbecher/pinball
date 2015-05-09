
#ifndef DISPLAY_MODULE_H
#define DISPLAY_MODULE_H

enum {
	ROWS = 16,
	COLUMNS = 8,

	PIXELS = ROWS * COLUMNS,

	IDX_RED = 0,
	IDX_GREEN = 1,
	LED_COLORS = 2
};

enum {
	C_BLACK = 0,
	C_RED = 1,
	C_GREEN = 2,
	C_YELLOW = 3,
	C_ORANGE = 4,
	C_DARK_RED = 5,
	C_DARK_GREEN = 6,
	C_BLOOD_ORANGE = 7,

	COLORS = 8,

	C_EOT = 0xff
};

unsigned int palette[LED_COLORS][COLORS];
unsigned char screen[LED_COLORS][ROWS][COLUMNS];

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
void render_selftest(unsigned long);
void clear_screen(void);


#endif // DISPLAY_MODULE_H

