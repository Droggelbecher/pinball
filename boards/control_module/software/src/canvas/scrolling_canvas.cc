
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
	 * ---+                      +--------
	 * +--|----------------------|-------+
	 * |  |                      |       |
	 * +--|----------------------|-------+
	 * ---+                      +--------
	 *                           offset
	 */


	if(c.column() >= offset_.column() && c.column() < offset_.column() + decorated_.size().column()) {
		decorated_.set(c - offset_, color);
	}

	else if(offset_.column() + decorated_.size().column() > size().column()
			&& c.column() < offset_.column() + decorated_.size().column() - size().column()) {
		Coordinate<> cc = (c + size() - offset_) % decorated_.size();

		decorated_.set(cc, color);
	}
}

uint8_t ScrollingCanvas::get(Coordinate<> c) const {
	return decorated_.get((c + offset_) % decorated_.size());
}

