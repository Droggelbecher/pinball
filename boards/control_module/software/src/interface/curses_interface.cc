
#include "curses_interface.h"

#include <ncurses.h>
#include <cassert>

const char CursesInterface::color_symbols[] = "    ####";
const uint8_t CursesInterface::color_fg[] = { COLOR_BLACK, COLOR_RED, COLOR_GREEN, COLOR_YELLOW, COLOR_RED,    COLOR_RED,   COLOR_GREEN, COLOR_YELLOW};
const uint8_t CursesInterface::color_bg[] = { COLOR_BLACK, COLOR_RED, COLOR_GREEN, COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK, COLOR_BLACK, COLOR_RED };


CursesInterface::CursesInterface(Coordinate<> size_) : buffer_(size_) {

	initscr();

	start_color();

	for(int i = 0; i < sizeof(color_fg)/sizeof(color_fg[0]); i++) {
		init_pair(i + 1, color_fg[i], color_bg[i]);
	}

	::clear();
	::move(0, 0);
}

CursesInterface::~CursesInterface() {
	endwin();
}

void CursesInterface::next_frame() {

	for(int row = 0; row < buffer_.canvas_size().row(); ++row) {
		for(int column = 0; column < buffer_.canvas_size().column(); ++column) {
			int col = buffer_.get_pixel(Coordinate<>(row, column));
			assert(col >= 0 && col < 8);
			attrset(COLOR_PAIR(col + 1));
			printw("%c%c", color_symbols[col], color_symbols[col]);
		}
		::move(row, 0);
	}
	::refresh();
}

void CursesInterface::set_pixel(Coordinate<> c, uint8_t color) {
	buffer_.set_pixel(c, color);
}

uint8_t CursesInterface::get_pixel(Coordinate<> c) const {
	return buffer_.get_pixel(c);
}

bool CursesInterface::get_switch(SwitchesIndex i) const {
	// TODO
	return false;
}

const CursesInterface::Bitset& CursesInterface::get_switches_bits() const {
	static Bitset bits;
	// TODO
	return bits;
}

void CursesInterface::set_solenoid(SolenoidsIndex i, bool v) {
	// TODO
}

void CursesInterface::set_lamp(LampsIndex i, bool v) {
	// TODO
}

