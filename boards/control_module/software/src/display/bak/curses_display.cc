
#include "curses_display.h"

#include <ncurses.h>
#include <cassert>

const char CursesDisplay::color_symbols[] = "    ####";
const uint8_t CursesDisplay::color_fg[] = { COLOR_BLACK, COLOR_RED, COLOR_GREEN, COLOR_YELLOW, COLOR_RED,    COLOR_RED,   COLOR_GREEN, COLOR_YELLOW};
const uint8_t CursesDisplay::color_bg[] = { COLOR_BLACK, COLOR_RED, COLOR_GREEN, COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK, COLOR_BLACK, COLOR_RED };


CursesDisplay::CursesDisplay(Coordinate<> size_) : buffer_(size_) {

	initscr();

	start_color();

	for(int i = 0; i < sizeof(color_fg)/sizeof(color_fg[0]); i++) {
		init_pair(i + 1, color_fg[i], color_bg[i]);
	}

	::clear();
	::move(0, 0);
}

CursesDisplay::~CursesDisplay() {
	endwin();
}

void CursesDisplay::next_frame() {

	for(int row = 0; row < buffer_.size().row(); ++row) {
		for(int column = 0; column < buffer_.size().column(); ++column) {
			int col = buffer_.get(Coordinate<>(row, column));
			assert(col >= 0 && col < 8);
			attrset(COLOR_PAIR(col + 1));
			printw("%c%c", color_symbols[col], color_symbols[col]);
		}
		::move(row, 0);
	}
	::refresh();
}

void CursesDisplay::set(Coordinate<> c, uint8_t color) {
	buffer_.set(c, color);
}

uint8_t CursesDisplay::get(Coordinate<> c) const {
	return buffer_.get(c);
}

