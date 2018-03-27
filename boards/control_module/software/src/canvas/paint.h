
#ifndef PAINT_H
#define PAINT_H

#include <cstdlib>

template<typename C>
void paint_random(C& canvas) {
	for(int row = 0; row < canvas.size().row(); ++row) {
		for(int column = 0; column < canvas.size().column(); ++column) {
			canvas.set_pixel(Coordinate<>(row, column), rand() % 8);
		}
	}
}

template<typename C>
void paint_pattern(C& canvas) {
	for(int row = 0; row < canvas.size().row(); ++row) {
		for(int column = 0; column < canvas.size().column(); ++column) {
			canvas.set_pixel(Coordinate<>(row, column), (row + column) % 8);
		}
	}
}

template<typename C>
auto paint_clear(C& canvas) -> decltype(canvas.clear(), void()) {
	canvas.clear();
}


/*
template<typename C>
void paint_clear(C& canvas) {
	for(int row = 0; row < canvas.size().row(); ++row) {
		for(int column = 0; column < canvas.size().column(); ++column) {
			canvas.set_pixel(Coordinate<>(row, column), 0);
		}
	}
}
*/

#endif // PAINT_H

