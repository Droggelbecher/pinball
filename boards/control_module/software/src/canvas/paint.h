
#ifndef PAINT_H
#define PAINT_H

#include <cstdlib>

template<typename C>
void paint_random(C& canvas) {
	for(int i = 0; i < canvas.buffer_length(); i++) {
		canvas.buffer()[i] = rand() % 8;
	}
}


template<typename C>
void paint_clear(C& canvas) {
	for(int row = 0; row < canvas.canvas_size().row(); ++row) {
		for(int column = 0; column < canvas.canvas_size().column(); ++column) {
			canvas.set_pixel(Coordinate<>(row, column), 0);
		}
	}
}

#endif // PAINT_H

