
#include <time.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <assert.h>

#include "config.h"
#include "display.h"


unsigned long display_frame;

unsigned char display_screen_[DISPLAY_SCREEN_BYTES + 1];
struct timeval display_frame_start_;
unsigned long display_time_passed_ = 0;
unsigned long display_last_report_ = 0;

struct timeval display_debug_start_;
unsigned long display_debug_start_frame_ = 0;

void display_setup(void) {
	gettimeofday(&display_frame_start_, 0);
	gettimeofday(&display_debug_start_, 0);
}

void display_start_frame(void) {
	gettimeofday(&display_frame_start_, 0);
}

void display_end_frame(void) {
	struct timeval now;
	gettimeofday(&now, 0);
	long delta = (now.tv_sec - display_frame_start_.tv_sec) * 1000
		+ (now.tv_usec - display_frame_start_.tv_usec) / 1000;

	if(delta >= 0) {
		usleep(1000.0 * ((1000.0 / DISPLAY_TARGET_FPS) - delta));
	}
	display_frame++;
}

void display_debug_fps(void) {
	struct timeval now;
	gettimeofday(&now, 0);
	long delta = (now.tv_sec - display_debug_start_.tv_sec) * 1000.0f
		+ (now.tv_usec - display_debug_start_.tv_usec) / 1000.0f;

	if(delta >= 10000) {
		printf("fps_avg %lf %lu\n", 1000.0f * (double)(display_frame - display_debug_start_frame_) / delta, (unsigned long)display_frame);
		fflush(stdout);

		gettimeofday(&display_debug_start_, 0);
		display_debug_start_frame_ = display_frame;
	}
}

void display_refresh(void) {
	spi_ss_activate_only(SPI_SS_PIN_DISPLAY);
	display_screen_[DISPLAY_SCREEN_BYTES] = C_EOT;
	spi_readwrite(sizeof(display_screen_), display_screen_, 0);
	spi_ss_deactivate_all();
}


unsigned char* display_screen(unsigned char row, unsigned char column) {
	const unsigned char module = column / DISPLAY_MODULE_COLUMNS;
	const unsigned char col = column % DISPLAY_MODULE_COLUMNS;

	// screen[module][colors][rows][columns]
	
	assert(0 <= row && row < DISPLAY_MODULE_ROWS);
	assert(0 <= col && col < DISPLAY_MODULE_COLUMNS);
	assert(0 <= module && module < DISPLAY_MODULE_COUNT);

	unsigned char *r = display_screen_ +
		module * (DISPLAY_MODULE_COLUMNS * DISPLAY_MODULE_ROWS) +
		row * (DISPLAY_MODULE_COLUMNS) +
		(DISPLAY_MODULE_COLUMNS - col - 1);

	assert(r >= display_screen_ && r < (display_screen_ + sizeof(display_screen_)));

	return r;
}

void display_render_clear(void) {
	memset(display_screen_, 0, DISPLAY_SCREEN_BYTES);
}

void display_render_fill(unsigned char color) {
	memset(display_screen_, color, DISPLAY_SCREEN_BYTES);
}

void display_render_selftest_fill() {
	enum { FPC = (int)((4.0 + DISPLAY_TARGET_FPS) / 4.0) };
	int color = (display_frame % (COLORS * FPC)) / FPC;

	display_render_fill((unsigned char)color);
}

void display_render_selftest_colorstripes() {
	int i;
	for(i = 0; i < DISPLAY_SCREEN_BYTES; i++) {
		display_screen_[i] = i % COLORS;
	}
}


/*
void display_render_colors() {
	enum { COLS = DISPLAY_MODULE_COUNT * DISPLAY_MODULE_COLUMNS,
		ROWS = DISPLAY_MODULE_ROWS };

	int row, col;

	for(row = 0; row < DISPLAY_MODULE_ROWS; row++) {
		for(col = 0; col < COLS; col++) {
			*display_screen(RED, row, col) = V[col % 8];
			*display_screen(GREEN, row, col) = V[row % 8];
		}
	}
}
*/

/*
void display_render() {
	int row, col;
	
	enum { MODE_ROW, MODE_COL };
	int mode = MODE_ROW;

	const double T = 0.5; // seconds per display of row/column
	const int line_frames = T * DISPLAY_TARGET_FPS;

*/


