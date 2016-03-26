
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
	memset(canvas.buffer(), 0, canvas.buffer_length());
}

#endif // PAINT_H

