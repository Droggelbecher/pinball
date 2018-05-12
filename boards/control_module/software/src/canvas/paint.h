
#ifndef PAINT_H
#define PAINT_H

#include <cstdlib>

namespace pinball {
	namespace canvas {

template<typename C>
void fill_random(C& canvas) {
	for(int row = 0; row < canvas.size().row(); ++row) {
		for(int column = 0; column < canvas.size().column(); ++column) {
			canvas.set_pixel(Coordinate<>(row, column), rand() % 8);
		}
	}
}

template<typename C>
void pattern(C& canvas) {
	for(int row = 0; row < canvas.size().row(); ++row) {
		for(int column = 0; column < canvas.size().column(); ++column) {
			canvas.set_pixel(Coordinate<>(row, column), (row + column) % 8);
		}
	}
}

template<typename C>
auto clear(C& canvas) -> decltype(canvas.buffer(), void()) {
	memset(canvas.buffer(), 0, canvas.buffer_length());
}

//void set_pi


} } // ns pinball::canvas

#endif // PAINT_H

