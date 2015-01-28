
#include <time.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <assert.h>

#include "config.h"
#include "display.h"

const unsigned char V[] = {
	0b00000000,
	0b00000001,
	0b00010001,
	0b00100101,
	0b01010101,
	0b01110101,
	0b11011011,
	0b11111011,
	0b11111111
};


unsigned long display_frame;

unsigned char display_screen_[DISPLAY_SCREEN_BYTES];
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
	spi_readwrite(sizeof(display_screen_), display_screen_, 0);
	spi_ss_deactivate_all();
}


unsigned char* display_screen(unsigned char color, unsigned char row, unsigned char column) {
	const unsigned char module = column / DISPLAY_MODULE_COLUMNS;
	const unsigned char col = column % DISPLAY_MODULE_COLUMNS;

	// screen[module][colors][rows][columns]
	
	assert(0 <= color && color < DISPLAY_MODULE_COLORS);
	assert(0 <= row && row < DISPLAY_MODULE_ROWS);
	assert(0 <= col && col < DISPLAY_MODULE_COLUMNS);
	assert(0 <= module && module < DISPLAY_MODULE_COUNT);

	unsigned char *r = display_screen_ +
		module * (DISPLAY_MODULE_COLUMNS * DISPLAY_MODULE_ROWS * DISPLAY_MODULE_COLORS) +
		color * (DISPLAY_MODULE_COLUMNS * DISPLAY_MODULE_ROWS) + 
		row * (DISPLAY_MODULE_COLUMNS) +
		(DISPLAY_MODULE_COLUMNS - col - 1);

	assert(r >= display_screen_ && r < (display_screen_ + sizeof(display_screen_)));

	return r;
}

void display_render_clear(void) {
	memset(display_screen_, 0, sizeof(display_screen_));
}
void display_render_monochrome(void) {
	memset(display_screen_, V[4], sizeof(display_screen_));
}

void display_render_fill(unsigned char red, unsigned char green) {
	int row, col;
	for(col = 0; col < DISPLAY_MODULE_COLUMNS; col++) {
		for(row = 0; row < DISPLAY_MODULE_ROWS; row++) {
			*display_screen(RED, row, col) = red;
			*display_screen(GREEN, row, col) = green;
		}
	}
}

void display_render_frame() {
	display_render_clear();
	int row, col;
	int phase = DISPLAY_PHASE(4.0, 16, display_frame);
	int v = (phase < 8) ? phase : 16 - phase;

	for(col = 0; col < DISPLAY_MODULE_COLUMNS * DISPLAY_MODULE_COUNT; col++) {
		*display_screen(RED, 0, col) = V[v];
		*display_screen(GREEN, DISPLAY_MODULE_ROWS - 1, col) = V[v];
	}

	for(row = 0; row < DISPLAY_MODULE_ROWS ; row++) {
		*display_screen(RED, row, 0) = V[v];
		*display_screen(GREEN, row, DISPLAY_MODULE_COLUMNS * DISPLAY_MODULE_COUNT - 1) = V[v];
	}
}


void display_render_gradient() {
	int row, col;

	const double T = 64; // seconds for a full cycle
	const int frames = T * DISPLAY_TARGET_FPS; // frames for a full cycle
	const int slow_phase_frames = frames / 16;
	const int fast_phase_frames = slow_phase_frames / 16;
	const int slow_phase = (display_frame % frames) / slow_phase_frames; 
	const int fast_phase = (display_frame % slow_phase_frames) / fast_phase_frames;

	for(row = 0; row < DISPLAY_MODULE_ROWS; row++) {
		for(col = 0; col < DISPLAY_MODULE_COUNT * DISPLAY_MODULE_COLUMNS; col++) {
			*display_screen(RED, (row + slow_phase) % 16, (fast_phase + 16) % 8) = V[(128 + row + (fast_phase < 8) ? fast_phase : (16 - fast_phase - 1)) % 8];
			*display_screen(GREEN, row, (col + slow_phase) % 8) = V[(128 + row - col + fast_phase ) % 8];
		}
	}
}

/*
void display_render() {
	int row, col;
	
	enum { MODE_ROW, MODE_COL };
	int mode = MODE_ROW;

	const double T = 0.5; // seconds per display of row/column
	const int line_frames = T * DISPLAY_TARGET_FPS;

*/


