
#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <string.h> // for memset
#include <time.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>

#include "config.h"
#include "spi.h"
#include "spi_ss.h"

enum {
	DISPLAY_SCREEN_BYTES =
		DISPLAY_MODULE_COUNT * DISPLAY_MODULE_ROWS *
		DISPLAY_MODULE_COLUMNS * DISPLAY_MODULE_COLORS,
};

enum {
	RED = 0, GREEN = 1
};

#define DISPLAY_PHASE(T, n, frame) ((frame % (unsigned long)((T) * DISPLAY_TARGET_FPS)) / (unsigned long)((T) * DISPLAY_TARGET_FPS / (n)))

// Brightness PWM patterns
extern const unsigned char V[];
extern unsigned char display_screen_[DISPLAY_SCREEN_BYTES];
extern unsigned long display_frame;

void display_setup(void);

void display_start_frame(void);
void display_end_frame(void);
void display_refresh(void);
void display_debug_fps(void);

unsigned char* display_screen(unsigned char color, unsigned char row, unsigned char column);

void display_render_clear(void);
void display_render_monochrome(void);
void display_render_gradient(void);
void display_render_frame(void);


#endif // __DISPLAY_H__

