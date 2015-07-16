
#include "display.h"
#include "timer.h"

void timer_compute(struct timer_state *ts) {
}

void timer_set(struct timer_state *ts, double target_time) {
	ts->target_frame_ = display_frame + (DISPLAY_TARGET_FPS * (double)target_time);
}

uint8_t timer_fired(struct timer_state *ts) {
	return display_frame >= ts->target_frame_;
}


