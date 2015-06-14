
#include <time.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <assert.h>

#include "config.h"
#include "spi_display.h"


unsigned long display_frame;

unsigned char display_screen_[DISPLAY_SCREEN_BYTES];
struct timeval display_frame_start_;
unsigned long display_time_passed_ = 0;
unsigned long display_last_report_ = 0;

struct timeval display_debug_start_;
unsigned long display_debug_start_frame_ = 0;

double display_avg_error_per_frame_ = 0;
#define DISPLAY_AVG_ERROR_PER_FRAME_ALPHA_ 0.2



double controllers_pi(double e, double *i) {
	const double Kp = 1.0;
	const double Tn = 1.0;

	*i += e;
	
	return Kp * (e + (1.0 / Tn) * (*i));
}




void display_setup(void) {
	gettimeofday(&display_frame_start_, 0);
	gettimeofday(&display_debug_start_, 0);
}

void display_start_frame(void) {
	gettimeofday(&display_frame_start_, 0);
}


// unit = ms
double delta = 1.0 / DISPLAY_TARGET_FPS;

void display_end_frame(void) {
	double err = (1.0 / DISPLAY_TARGET_FPS) - delta;
	/*if(err < 0) { err = 0.0; }*/


	double wait = controllers_pi(err, &display_avg_error_per_frame_);
	if(wait > 0) {
		usleep(1000.0 * 1000.0 * wait);
	}

	struct timeval now;
	gettimeofday(&now, 0);
	delta = (now.tv_sec - display_frame_start_.tv_sec)
		+ (now.tv_usec - display_frame_start_.tv_usec) / 1000000.0;

	/*if(delta >= 0) {*/
	/*if(1000.0 * ((1000.0 / DISPLAY_TARGET_FPS) - delta - display_avg_error_per_frame_) >= 0) {*/
		/*usleep(1000.0 * ((1000.0 / DISPLAY_TARGET_FPS) - delta - display_avg_error_per_frame_));*/
	/*}*/
	display_frame++;
}

void display_debug_fps(void) {
	struct timeval now;
	gettimeofday(&now, 0);
	long delta = (now.tv_sec - display_debug_start_.tv_sec) * 1000.0f
		+ (now.tv_usec - display_debug_start_.tv_usec) / 1000.0f;

	double fps = 1000.0f * (display_frame - display_debug_start_frame_) / delta;
	/*display_avg_error_per_frame_ += DISPLAY_AVG_ERROR_PER_FRAME_ALPHA_ * (*/
			/*(1000.0 / fps) - (1000.0 / DISPLAY_TARGET_FPS)*/
	/*);*/

	if(delta >= 1000) {

		printf("fps_avg %lf fr# %lu err %lf\n", fps, (unsigned long)display_frame, display_avg_error_per_frame_);
		fflush(stdout);

		gettimeofday(&display_debug_start_, 0);
		display_debug_start_frame_ = display_frame;
	}
}

void display_refresh(void) {
	spi_ss_activate_only(SPI_SS_PIN_DISPLAY);

	int i;
	for(i = 0; i < DISPLAY_MODULE_COUNT; i++) {
		spi_readwrite(DISPLAY_MODULE_ROWS * DISPLAY_MODULE_COLUMNS, display_screen_ + i * (DISPLAY_MODULE_ROWS * DISPLAY_MODULE_COLUMNS), 0);
		usleep(10);
	}
	

	spi_ss_deactivate_all();
}

uint8_t display_sane(uint8_t row, uint8_t column) {
	const unsigned char module = column / DISPLAY_MODULE_COLUMNS;
	const unsigned char col = column % DISPLAY_MODULE_COLUMNS;

	return (
		(0 <= row && row < DISPLAY_MODULE_ROWS) &&
		(0 <= col && col < DISPLAY_MODULE_COLUMNS) &&
		(0 <= module && module < DISPLAY_MODULE_COUNT)
	);
}

unsigned char* display_screen(uint8_t row, uint8_t column) {
	const unsigned char module = column / DISPLAY_MODULE_COLUMNS;
	const unsigned char col = column % DISPLAY_MODULE_COLUMNS;

	// screen[module][colors][rows][columns]
	
	assert(0 <= row && row < DISPLAY_MODULE_ROWS);
	assert(0 <= col && col < DISPLAY_MODULE_COLUMNS);
	assert(0 <= module && module < DISPLAY_MODULE_COUNT);

	unsigned char *r = display_screen_ +
		(DISPLAY_MODULE_COUNT - module - 1) * (DISPLAY_MODULE_COLUMNS * DISPLAY_MODULE_ROWS) +
		row * (DISPLAY_MODULE_COLUMNS) +
		(DISPLAY_MODULE_COLUMNS - col - 1);

	assert(r >= display_screen_ && r < (display_screen_ + sizeof(display_screen_)));

	return r;
}

