
#ifndef CANVAS_H
#define CANVAS_H

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <cassert>
#include <cstring> // memcpy
#include "coordinate.h"

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

template<typename C>
auto clear(C& canvas) -> decltype(canvas.clear(), void()) {
	canvas.clear();
}

template<typename C>
uint8_t get_pixel(const C& canvas, Coordinate<> c) {
	return canvas.get_pixel(c);
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

template<typename Ca, typename Cb>
bool operator==(const Ca& a, const Cb& b) {
	if(a.size() != b.size()) {
		return false;
	}

	for(int row = 0; row < a.size().row(); row++) {
		for(int column = 0; column < a.size().column(); column++) {
			if(a.get_pixel({row, column}) != b.get_pixel({row, column})) {
				return false;
			}
		}
	}

	return true;
}

template<typename Ca, typename Cb>
bool operator!=(const Ca& a, const Cb& b) {
	return !(a == b);
}

template<typename C>
auto operator<<(std::ostream& os, const C& c) -> decltype(c.size(), c.get_pixel({0, 0}), os) {
	os << "Canvas(" << c.size() << "):\n";
	for(int row = 0; row < c.size().row(); row++) {
		for(int column = 0; column < c.size().column(); column++) {
			os << " " << std::setw(3) << (int)get_pixel(c, {row, column});
		}
		os << "\n";
	}
	os << std::endl;
	return os;
}

typedef enum {
	COLUMN_FIRST,
	ROW_FIRST,
	NONE,
	OTHER
} DataOrder;

/**
 * Copy the rectangle ($start, $end) from $a to $b at location $target.
 */
template<typename CanvasA, typename CanvasB>
auto blit(const CanvasA& a, CanvasB& b, Coordinate<> start, Coordinate<> end, Coordinate<> target) -> decltype(a.buffer(), b.buffer(), void()) {

	static_assert(CanvasA::data_order == CanvasB::data_order, "Can only blit with equal data orders");

	static_assert(CanvasA::data_order == COLUMN_FIRST, "Blit only implemented for COLUMN_FIRST currently");

	assert(0 <= start.row() && start.row() < a.size().row());
	assert(0 <= start.column() && start.column() < a.size().column());
	assert(0 < end.row() && end.row() <= a.size().row());
	assert(0 < end.column() && end.column() <= a.size().column());

	assert(0 <= target.row() && target.row() < b.size().row());
	assert(0 <= target.column() && target.column() < b.size().column());

	Coordinate<> target_end = target - start + end;
	assert(0 < target_end.row() && target_end.row() <= b.size().row());
	assert(0 < target_end.column() && target_end.column() <= b.size().column());

	int n_a = a.size().column();
	int n_b = b.size().column();
	int len = end.column() - start.column();

	const uint8_t *buf_a = a.buffer() + start.column();
	uint8_t *buf_b = b.buffer() + target.column();

	/*
	 * +-----------+
	 * |           |
	 * |    S---+  |
	 * |    |   |  |
	 * |    +---E  |
	 * |           |
	 * +-----------+
	 */

	// copy row by row
	for(int row = start.row(); row < end.row(); ++row) {
		// dest, src, n
		memcpy(
			buf_b + (row - start.row() + target.row()) * n_b,
			buf_a + row * n_a,
			len
			);
	}
} // blit()

} } // ns pinball::canvas

#endif // CANVAS_H

