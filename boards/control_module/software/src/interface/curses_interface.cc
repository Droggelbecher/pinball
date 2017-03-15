
#include "curses_interface.h"

#include <ncurses.h>
#include <cassert>

const char CursesInterface::color_symbols[] = "    ####";
const uint8_t CursesInterface::color_fg[] = { COLOR_WHITE, COLOR_RED, COLOR_GREEN, COLOR_YELLOW, COLOR_RED,    COLOR_RED,   COLOR_GREEN, COLOR_YELLOW};
const uint8_t CursesInterface::color_bg[] = { COLOR_BLACK, COLOR_RED, COLOR_GREEN, COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK, COLOR_BLACK, COLOR_RED };


CursesInterface::CursesInterface(Coordinate<> size_) : buffer_(size_) {

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


	// "display"

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

	// "solenoids"
	print_state(" FL L ", solenoids().get(FLIPPER_LEFT), row, 2);
	print_state(" FL R ", solenoids().get(FLIPPER_RIGHT), row, 9);

	row += 2;

	::move(row, 0);

	::attrset(COLOR_PAIR(0));
	::printw("1 FL L\n");
	::printw("2 FL R\n");

	::refresh();

	handle_keys();
}

void CursesInterface::handle_keys() {

	struct Key {
		Key(int toggle, int flip, Index idx, bool state) :
			toggle(toggle), flip(flip), idx(idx), state(state) { }
		int toggle;
		int flip;
		Index idx;
		bool state;
	};

	static Key key_table[] = {
		Key('1', '!', FLIPPER_LEFT, false),
		Key('2', '@', FLIPPER_RIGHT, false)
	};

	// clear all key states
	//for(int i = 0; i < sizeof(key_table) / sizeof(key_table[0]); i++) {
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

void CursesInterface::print_state(const char *s, bool active, int row, int column) {
	::move(row, column);
	::attrset((active ? COLOR_PAIR(7) : COLOR_PAIR(6)) | A_REVERSE);
	::printw("%s", s);
}

void CursesInterface::set_pixel(Coordinate<> c, uint8_t color) {
	buffer_.set_pixel(c, color);
}

uint8_t CursesInterface::get_pixel(Coordinate<> c) const {
	return buffer_.get_pixel(c);
}

