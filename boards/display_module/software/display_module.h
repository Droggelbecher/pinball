
#ifndef DISPLAY_MODULE_H
#define DISPLAY_MODULE_H


#define DDR_TLC5940 DDRC
#define PORT_TLC5940 PORTC

#define DDR_MOSFETS DDRD
#define PORT_MOSFETS PORTD

enum {
	ROWS = 16,
	COLUMNS = 8,

	PIXELS = ROWS * COLUMNS,

	IDX_RED = 0,
	IDX_GREEN = 1,
	COLORS = 2,

	P_SIN = 5,
	P_SCLK = 4,
	P_XLAT = 3,
	P_BLANK = 2,
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

	LED_COLORS = 8,

	C_EOT = 0xff
};


typedef struct {
	unsigned char row;
	unsigned char column;
	unsigned char color;
} ScreenIndex;



unsigned int palette[LED_COLORS][COLORS];
unsigned char screen[COLUMNS * ROWS * COLORS];



int main(void);
void setup_spi(void);
void enable_next(void);
void disable_next(void);
void setup_tlc5940(void);
void setup_mosfets(void);
void output_column(int);
void output_screen(void);
void render_selftest(unsigned long);
void clear_screen(void);


#endif // DISPLAY_MODULE_H

