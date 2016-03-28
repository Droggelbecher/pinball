
#include "scrolling_canvas.h"

#include <cassert>
#include <iostream>

ScrollingCanvas::ScrollingCanvas(Canvas& decorated, Coordinate<double> speed)
	: decorated_(decorated), speed_(speed)
{
}

void ScrollingCanvas::next_frame() {
	offset_ += speed_;
	offset_ %= size();
}

void ScrollingCanvas::resize(Coordinate<> new_size) {
	size_ = new_size;
	offset_ %= size();
}

void ScrollingCanvas::set(Coordinate<> c, uint8_t color) {
	/*
	 *  +-----------------------------------+
	 *  | A|                         |  B   |
	 *  |  |                         |      |
	 *  |--+                         +------|
	 *  |                                   |
	 *  |                                   |
	 *  |                                   |
	 *  |                                   |
	 *  |                          offset_  |
	 *  |--+                         X------|-----+
	 *  | C|                         |  D   |     |
	 *  +-----------------------------------X     |
	 *           [R1]                |    size()  |
	 *                               |            |
	 *                               +------------+
	 *  +---------+                       [R2]
	 *  |   D  : C|
	 *  |......:..|
	 *  |   B  : A|
	 *  |      :  |
	 *  +---------+
	 *     [R2]   decorated_.size()
	 */
	
	assert(size().contains(c));

	// Is coordinate c in part A, B, C or D? (if existant)

	// if c is in A, B or C, add size() so it is in R2
	// if c is in D, nothing will happen
	// if c is neither in A, B, C or D it will land outside of R2

	if(c.column() < offset_.column()) {
		c += Coordinate<>(0, size().column());
	}

	if(c.row() < offset_.row()) {
		c += Coordinate<>(size().row(), 0);
	}

	// substract offset_ to make the coordinate relative to decorated_.
	// If it is inside, pass it through

	if(decorated_.size().contains(c - offset_)) {
		decorated_.set(c - offset_, color);
	}
}

uint8_t ScrollingCanvas::get(Coordinate<> c) const {
	return decorated_.get((c + offset_) % decorated_.size());
}

