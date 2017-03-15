
#include "curses_interface.h"

#include <ncurses.h>
#include <cassert>

const char CursesInterface::color_symbols[] = "    ####";
const uint8_t CursesInterface::color_fg[] = { COLOR_WHITE, COLOR_RED, COLOR_GREEN, COLOR_YELLOW, COLOR_RED,    COLOR_RED,   COLOR_GREEN, COLOR_YELLOW};
const uint8_t CursesInterface::color_bg[] = { COLOR_BLACK, COLOR_RED, COLOR_GREEN, COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK, COLOR_BLACK, COLOR_RED };


CursesInterface::CursesInterface(Coordinate<> size_) : buffer_(size_) {

	// Switches are active low, thus default them to high
	switches().set();

	::initscr();
	::nodelay(stdscr, TRUE); // allow non-blocking keyboard input
	::start_color();

	for(int i = 0; i < sizeof(color_fg)/sizeof(color_fg[0]); i++) {
		::init_pair(i + 1, color_fg[i], color_bg[i]);
	}

	::clear();
	::move(0, 0);
}

CursesInterface::~CursesInterface() {
	endwin();
}

void CursesInterface::next_frame() {

	struct SolenoidState {
		const char *str;
		Index index;
	};

	static SolenoidState solenoid_states[] = {
		{ "FL L", FLIPPER_LEFT },
		{ "FL R", FLIPPER_RIGHT },
		{ "DTB0", DTB0 },
		{ "SS 0", SLINGSHOT0 },
		{ "SS 1", SLINGSHOT1 },
		{ "BMP0", BUMPER0 },
		{ "BMP1", BUMPER1 },
		{ "BMP2", BUMPER2 },
		{ "BLRT", BALL_RETURN },
	};


	// Display emulation

	int row = 0;
	for( ; row < buffer_.canvas_size().row(); ++row) {
		for(int column = 0; column < buffer_.canvas_size().column(); ++column) {
			int col = buffer_.get_pixel(Coordinate<>(row, column));
			assert(col >= 0 && col < 8);
			::attrset(COLOR_PAIR(col + 1));
			::printw("%c%c", color_symbols[col], color_symbols[col]);
		}
		::move(row, 0);
	}

	row += 2;
	int column = 2;

	// Solenoid states

	for(SolenoidState& s: solenoid_states) {
		::move(row, column);
		::attrset(solenoids().get(s.index) ? (COLOR_PAIR(7) | A_REVERSE) : COLOR_PAIR(0));
		::printw(" %s ", s);
		column += 7;
	}
	::attrset(COLOR_PAIR(0));


	row += 2;

	::move(row, 0);

	::refresh();

	handle_keys();
}

void CursesInterface::handle_keys() {

	struct Key {
		int toggle;
		int flip;
		const char *str;
		Index idx;
		bool state;
	};

	static Key key_table[] = {
		{ '1', '!', "FL_L ", FLIPPER_LEFT,  true },
		{ '2', '@', "FL_R ", FLIPPER_RIGHT, true },
		{ '3', '#', "DTB00", DTB0_0, true },
		{ '4', '$', "DTB01", DTB0_1, true },
		{ '5', '%', "DTB02", DTB0_2, true },
		{ '6', '^', "DTB03", DTB0_3, true },
		{ '7', '&', "DTB04", DTB0_4, true },
		{ '8', '*', "SS_0 ", SLINGSHOT0, true },
		{ '9', '(', "SS_1 ", SLINGSHOT1, true },
		{ '0', ')', "BMP_0", BUMPER0, true },
		{ '[', '{', "BMP_1", BUMPER1, true },
		{ ']', '}', "BMP_2", BUMPER2, true },

		{ '\'', '"', "BLO ", BALL_OUT, true },
	};

	::printw("  ");
	int i = 0;
	for(const Key& key: key_table) {
		::attrset(!key.state ? (COLOR_PAIR(0) | A_REVERSE) : COLOR_PAIR(0));
		::printw(" %c%c:%s ", (char)key.toggle, (char)key.flip, key.str);
		::attrset(COLOR_PAIR(0));
		::printw(" ");
		if(++i == 12) {
			::printw("\n  ");
			i = 0;
		}
	}

	// clear all key states
	for(const Key& key: key_table) {
		switches().set(key.idx, key.state);
	}

	int ch = getch();
	if(ch == ERR) {
		return;
	}

	for(Key& key: key_table) {
		if(ch == key.toggle) {
			switches().set(key.idx, !key.state);
			break;
		}
		else if(ch == key.flip) {
			key.state = !key.state;
			switches().set(key.idx, key.state);
			break;
		}
	}

}

void CursesInterface::set_pixel(Coordinate<> c, uint8_t color) {
	buffer_.set_pixel(c, color);
}

uint8_t CursesInterface::get_pixel(Coordinate<> c) const {
	return buffer_.get_pixel(c);
}

