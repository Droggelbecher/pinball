
#include "render.h"
#include "display.h"

void render_clear(void) {
	memset(display_screen_, 0, DISPLAY_SCREEN_BYTES);
}

void render_fill(unsigned char color) {
	memset(display_screen_, color, DISPLAY_SCREEN_BYTES);
}

void render_selftest_fill() {
	enum { FPC = (int)((4.0 + DISPLAY_TARGET_FPS) / 4.0) };
	int color = (display_frame % (COLORS * FPC)) / FPC;

	render_fill((unsigned char)color);
}

void render_selftest_colorstripes() {
	enum { FPC = (int)((4.0 + DISPLAY_TARGET_FPS) / 4.0) };
	int i;
	for(i = 0; i < DISPLAY_SCREEN_BYTES; i++) {
		display_screen_[i] = (i + (display_frame / FPC)) % COLORS;
	}
}

