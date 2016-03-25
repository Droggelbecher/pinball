
#include "display.h"

#include <ncurses.h>

Display::Display() {
	initscr();
	printw("Hello World :-)");
	refresh();
}

Display::~Display() {
	endwin();
}

