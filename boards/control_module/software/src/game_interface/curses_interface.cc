
#include "curses_interface.h"

#include <ncurses.h>
#include <cassert>

namespace pinball {

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
		lamps_(decorated_.lamps()),
		solenoids_(decorated_.solenoids(), Solenoids::PASS_THROUGH)
{


	// Switches are active low, thus default them to high
	//switches().set();

	::initscr();
	::nodelay(stdscr, TRUE); // allow non-blocking keyboard input
	::cbreak(); // do not buffer keys
	::keypad(stdscr, TRUE); // Read eg F-keys as well
	::noecho();
	::start_color();

	for(size_t i = 0; i < sizeof(color_fg)/sizeof(color_fg[0]); i++) {
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
	using Sol = typename Solenoids::Index;
	using Sw = typename Switches::Index;

	decorated().next_frame(dt);

	// Direct control & feedback over solenoids

	struct SolenoidState {
		const int key;
		const char *str;
		Sol index;
	};

	static const int enable_solenoid_control_key = KEY_F(1);
	static bool enable_solenoid_control = false;
	static SolenoidState solenoid_states[] = {
		// Dvorak lower key row
		{ 'a', "FL L", Sol::FLIPPER_LEFT },
		{ 'o', "FL R", Sol::FLIPPER_RIGHT },
		{ 'e', "DTB0", Sol::DTB0 },
		{ 'u', "SS 0", Sol::SLINGSHOT0 },
		{ 'i', "SS 1", Sol::SLINGSHOT1 },
		{ 'd', "BMP0", Sol::BUMPER0 },
		{ 'h', "BMP1", Sol::BUMPER1 },
		{ 't', "BMP2", Sol::BUMPER2 },
		{ 'n', "BLRT", Sol::BALL_RETURN },
		{ 's', "AUX0", Sol::AUX0 },
		{ '-', "AUX1", Sol::AUX1 }
	};

	// Direct control & feedback over switch states

	struct Key {
		int toggle;
		int flip;
		const char *str;
		Sw idx;
		bool state;
	};

	static Key key_table[] = {
		// Dvorak Keyboard Layout
		// tog flip name     idx                 state
		{ '1', '!', "FL_L ", Sw::FLIPPER_LEFT,  false },
		{ '2', '@', "FL_R ", Sw::FLIPPER_RIGHT, false },
		{ '3', '#', "DTB00", Sw::DTB0_0,        false },
		{ '4', '$', "DTB01", Sw::DTB0_1,        false },
		{ '5', '%', "DTB02", Sw::DTB0_2,        false },
		{ '6', '^', "DTB03", Sw::DTB0_3,        false },
		{ '7', '&', "DTB04", Sw::DTB0_4,        false },
		{ '8', '*', "SS_0 ", Sw::SLINGSHOT0,    false },
		{ '9', '(', "SS_1 ", Sw::SLINGSHOT1,    false },
		{ '0', ')', "BMP_0", Sw::BUMPER0,       false },
		{ '[', '{', "BMP_1", Sw::BUMPER1,       false },
		{ ']', '}', "BMP_2", Sw::BUMPER2,       false },

		{ '\'', '"', "BLO ", Sw::BALL_OUT,      false },
		{ ',',  '<', "HOL0", Sw::HOLE0,         false },
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
	::printw("\n\n  ");

	// Display switch states
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
	::printw("\n\n  ");

	// Display Solenoid states
	attrset(enable_solenoid_control ? (COLOR_PAIR(7) | A_REVERSE) : COLOR_PAIR(0));
	::printw(" F1:ENABLE ");
	attrset(COLOR_PAIR(0));
	::printw(" ");
	for(SolenoidState& s: solenoid_states) {
		attrset(solenoids().decorated().get(s.index) ? (COLOR_PAIR(7) | A_REVERSE) : COLOR_PAIR(0));
		::printw(" %c:%s ", s.key, s.str);
		attrset(COLOR_PAIR(0));
		::printw(" ");
	}
	attrset(COLOR_PAIR(0));
	::printw("\n\n");

	for(std::string line: logger_) {
		::printw("   %s\n", line.c_str());
	}

	::refresh();

	// reset all key states
	for(const Key& key: key_table) {
		switches().set(key.idx, key.state);
	}

	// read & process key
	int ch = getch();

	if(ch == enable_solenoid_control_key) {
		enable_solenoid_control = !enable_solenoid_control;
		if(enable_solenoid_control) {
			solenoids().set_mode(Solenoids::READ_ONLY);
		}
		else {
			solenoids().set_mode(Solenoids::PASS_THROUGH);
		}
	}

	// Is this a switch-emulating key?
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

	// Is this a solenoid-controlling key?
	if(enable_solenoid_control) {
		for(SolenoidState& solenoid: solenoid_states) {
			if(ch == solenoid.key) {
				solenoids().decorated().set(solenoid.index, true);
				//break;
			}
			else {
				solenoids().decorated().set(solenoid.index, false);
			}
		} // for solenoid
	} // if enable solenoid contral
} // next_frame()

} // ns pinball
