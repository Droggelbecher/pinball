
#include <stdio.h>
#include <stdlib.h> // abs()
#include <assert.h>

#include "display.h"
#include "scroll.h"
#include "utils.h" // sgn()

void scroll_compute(struct scroll_state *ss) {
	if(ss->speed_columns && display_frame >= ss->next_column_frame_) {
		ss->current_column += sgn(ss->speed_columns);
		ss->next_column_frame_ = display_frame + DISPLAY_TARGET_FPS / abs(ss->speed_columns);
		assert(ss->next_column_frame_ >= display_frame);
	}
	if(ss->speed_rows && display_frame >= ss->next_row_frame_) {
		ss->current_row += sgn(ss->speed_rows);
		ss->next_row_frame_ = display_frame + DISPLAY_TARGET_FPS / abs(ss->speed_rows);
		assert(ss->next_row_frame_ >= display_frame);
	}
}

void scroll_reset(struct scroll_state *ss) {
	ss->current_column = ss->start_column;
	ss->current_row = ss->start_row;
	ss->next_column_frame_ = display_frame + DISPLAY_TARGET_FPS / abs(ss->speed_columns);
	ss->next_row_frame_ = display_frame + DISPLAY_TARGET_FPS / abs(ss->speed_rows);
}

