
#ifndef __SPI_DISPLAY_H__
#define __SPI_DISPLAY_H__

#include <string.h> // for memset
#include <time.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <stdint.h>

#include "config.h"
#include "spi.h"
#include "spi_ss.h"

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


enum {
	DISPLAY_SCREEN_BYTES =
		DISPLAY_MODULE_COUNT * DISPLAY_MODULE_ROWS *
		DISPLAY_MODULE_COLUMNS,
};

#define DISPLAY_PHASE(T, n, frame) ((frame % (unsigned long)((T) * DISPLAY_TARGET_FPS)) / (unsigned long)((T) * DISPLAY_TARGET_FPS / (n)))

extern unsigned char display_screen_[DISPLAY_SCREEN_BYTES + 1];
extern unsigned long display_frame;

void display_setup(void);

void display_start_frame(void);
void display_end_frame(void);
void display_refresh(void);
void display_debug_fps(void);

uint8_t display_sane(uint8_t row, uint8_t column);
unsigned char* display_screen(uint8_t row, uint8_t column);

#endif // __SPI_DISPLAY_H__

