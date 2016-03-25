
#ifndef PAINT_H
#define PAINT_H

#include <cstdlib>

template<typename C>
void paint_random(C& canvas) {

	for(int i = 0; i < canvas.buffer_length(); i++) {
		canvas.buffer()[i] = rand() % 8;
	}
}


#endif // PAINT_H

