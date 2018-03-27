
#include "curses_interface.h"

#include <ncurses.h>
#include <cassert>

template<typename TDecorated>
const char CursesInterface<TDecorated>::color_symbols[] = "    ####";

template<typename TDecorated>
const uint8_t CursesInterface<TDecorated>::color_fg[] = { COLOR_WHITE, COLOR_RED, COLOR_GREEN, COLOR_YELLOW, COLOR_RED,    COLOR_RED,   COLOR_GREEN, COLOR_YELLOW};

template<typename TDecorated>
const uint8_t CursesInterface<TDecorated>::color_bg[] = { COLOR_BLACK, COLOR_RED, COLOR_GREEN, COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK, COLOR_BLACK, COLOR_RED };

template<typename TDecorated>
CursesInterface<TDecorated>::CursesInterface(Coordinate<> size_, TDecorated& decorated) :
		decorated_(decorated),
		internal_canvas_(size_),
		canvas_ { internal_canvas_, decorated_.canvas() },
		switches_(decorated_.switches()),
		lamps_(decorated_.lamps())
{


	// Switches are active low, thus default them to high
	//switches().set();

	::initscr();
	::nodelay(stdscr, TRUE); // allow non-blocking keyboard input
	::start_color();

	for(int i = 0; i < sizeof(color_fg)/sizeof(color_fg[0]); i++) {
		::init_pair(i + 1, color_fg[i], color_bg[i]);
	}

	::clear();
	::move(0, 0);
}

template<typename TDecorated>
CursesInterface<TDecorated>::~CursesInterface() {
	endwin();
}

template<typename TDecorated>
void CursesInterface<TDecorated>::next_frame(double dt) {
	using Idx = typename Solenoids::Index;
	decorated().next_frame(dt);

	struct SolenoidState {
		const char *str;
		Idx index;
	};

	static SolenoidState solenoid_states[] = {
		{ "FL L", Idx::FLIPPER_LEFT },
		{ "FL R", Idx::FLIPPER_RIGHT },
		{ "DTB0", Idx::DTB0 },
		{ "SS 0", Idx::SLINGSHOT0 },
		{ "SS 1", Idx::SLINGSHOT1 },
		{ "BMP0", Idx::BUMPER0 },
		{ "BMP1", Idx::BUMPER1 },
		{ "BMP2", Idx::BUMPER2 },
		{ "BLRT", Idx::BALL_RETURN },
	};


	// Display emulation
	// 
	int row = 0;
	for( ; row < canvas().size().row(); ++row) {
		::move(row, 0);
		for(int column = 0; column < canvas().size().column(); ++column) {
			int col = canvas().get_pixel(Coordinate<>(row, column));
			assert(col >= 0 && col < 8);
			attrset(COLOR_PAIR(col + 1));
			::printw("%c%c", color_symbols[col], color_symbols[col]);
		}
	}
	row += 2;
	int column = 2;

	// Display Solenoid states
	for(SolenoidState& s: solenoid_states) {
		::move(row, column);
		attrset(solenoids().get(s.index) ? (COLOR_PAIR(7) | A_REVERSE) : COLOR_PAIR(0));
		::printw(" %s ", s);
		column += 7;
	}
	attrset(COLOR_PAIR(0));
	row += 2;
	::move(row, 0);
	::refresh();
	handle_keys();
}

template<typename TDecorated>
void CursesInterface<TDecorated>::handle_keys() {
	using Idx = typename Switches::Index;
	struct Key {
		int toggle;
		int flip;
		const char *str;
		Idx idx;
		bool state;
	};

	static Key key_table[] = {
		// Dvorak Keyboard Layout
		// tog flip name     idx                 state
		{ '1', '!', "FL_L ", Idx::FLIPPER_LEFT,  false },
		{ '2', '@', "FL_R ", Idx::FLIPPER_RIGHT, false },
		{ '3', '#', "DTB00", Idx::DTB0_0,        false },
		{ '4', '$', "DTB01", Idx::DTB0_1,        false },
		{ '5', '%', "DTB02", Idx::DTB0_2,        false },
		{ '6', '^', "DTB03", Idx::DTB0_3,        false },
		{ '7', '&', "DTB04", Idx::DTB0_4,        false },
		{ '8', '*', "SS_0 ", Idx::SLINGSHOT0,    false },
		{ '9', '(', "SS_1 ", Idx::SLINGSHOT1,    false },
		{ '0', ')', "BMP_0", Idx::BUMPER0,       false },
		{ '[', '{', "BMP_1", Idx::BUMPER1,       false },
		{ ']', '}', "BMP_2", Idx::BUMPER2,       false },

		{ '\'', '"', "BLO ", Idx::BALL_OUT,      false },
		{ ',',  '<', "HOL0", Idx::HOLE0,         false },
	};

	::printw("  ");
	int i = 0;
	for(const Key& key: key_table) {
		//attrset(key.state ? (COLOR_PAIR(0) | A_REVERSE) : COLOR_PAIR(0));
		attrset(switches().get(key.idx) ? (COLOR_PAIR(0) | A_REVERSE) : COLOR_PAIR(0));
		::printw(" %c%c:%s ", (char)key.toggle, (char)key.flip, key.str);
		attrset(COLOR_PAIR(0));
		::printw(" ");
		if(++i == 12) {
			::printw("\n  ");
			i = 0;
		}
	}

	// reset all key states
	for(const Key& key: key_table) {
		switches().set(key.idx, key.state);
	}

	// read & process key
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

//template<typename TDecorated>
//void CursesInterface<TDecorated>::set_pixel(Coordinate<> c, uint8_t color) {
	//decorated().set_pixel(c, color);
	//canvas().set_pixel(c, color);
//}

//template<typename TDecorated>
//uint8_t CursesInterface<TDecorated>::get_pixel(Coordinate<> c) const {
	//return canvas().get_pixel(c);
//}

