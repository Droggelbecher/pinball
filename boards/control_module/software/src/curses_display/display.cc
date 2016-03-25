
#include "display.h"

#include <ncurses.h>

Display::Display(Coordinate<> size_) : buffer_(size_) {

	initscr();

	start_color();

	init_pair(1, COLOR_GREEN, COLOR_GREEN);
	init_pair(2, COLOR_MAGENTA, COLOR_MAGENTA);
	init_pair(3, COLOR_RED, COLOR_RED);
	init_pair(4, COLOR_BLUE, COLOR_BLUE);
	init_pair(5, COLOR_CYAN, COLOR_CYAN);
	init_pair(6, COLOR_YELLOW, COLOR_YELLOW);
	init_pair(7, COLOR_GREEN, COLOR_GREEN);
	init_pair(8, COLOR_MAGENTA, COLOR_MAGENTA);
	init_pair(9, COLOR_RED, COLOR_RED);

	::clear();
	::move(0, 0);
	//printw("Hello World :-)");
	//refresh();

}

Display::~Display() {
	endwin();
}

void Display::refresh() {

	for(int row = 0; row < buffer_.size().row(); ++row) {
		for(int column = 0; column < buffer_.size().column(); ++column) {
			attrset(COLOR_PAIR(buffer_.get(row, column) + 1));
			printw("  ");
		}
		::move(row, 0);
	}
	::refresh();
}
