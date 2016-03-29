
#ifndef PAINT_H
#define PAINT_H

#include <cstdlib>

template<typename C>
void paint_random(C& canvas) {
	for(int i = 0; i < canvas.buffer_length(); i++) {
		canvas.buffer()[i] = rand() % 8;
	}
}


// TODO: add an additional interface for buffer() and buffer_length()
// (if we figure we need it at all)
//template<typename C>
//void paint_clear(C& canvas) {
	//memset(canvas.buffer(), 0, canvas.buffer_length());
//}

template<typename C>
void paint_clear(C& canvas) {
	for(int row = 0; row < canvas.size().row(); ++row) {
		for(int column = 0; column < canvas.size().column(); ++column) {
			canvas.set(Coordinate<>(row, column), 0);
		}
	}
	//memset(canvas.buffer(), 0, canvas.buffer_length());
}

#endif // PAINT_H

