
#ifndef PAINT_H
#define PAINT_H

#include <cstdlib>

namespace pinball::canvas {

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

template<typename C>
uint8_t get_pixel(const C& canvas, Coordinate<> c, uint8_t color) {
	return canvas.get_pixel(c, color);
}

template<typename C>
void set_pixel(C& canvas, Coordinate<> c, uint8_t color) {
	canvas.set_pixel(c, color);
}

template<typename C>
void next_frame(C& canvas, double dt) {
	canvas.next_frame(dt);
}

template<typename C>
Coordinate<> size(const C& canvas) {
	return canvas.size();
}

template<typename C>
void resize(C& canvas, Coordinate<> new_size) {
	canvas.resize(new_size);
}


} // namespace::canvas

#endif // PAINT_H

