
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
		//std::cout
			//<< decorated_.size().row() << " " << decorated_.size().column() << " "
			//<< size().row() << " " << size().column() << " "
			//<< c.row() << " " << c.column() << " "
			//<< offset_.row() << " " << offset_.column() << std::endl;

		//assert( decorated_.size().contains(c + size() - offset_) );
		Coordinate<> cc = (c + size() - offset_) % decorated_.size();

		if(decorated_.size().contains(cc)) {
		decorated_.set(cc, color);
		}
				//c + size() - offset_ - decorated_.size(), color);
		//decorated_.set(
				//Coordinate<>(c.row(), c.column() + size().column() - offset_.column()), color);
		//decorated_.set(c, color);
	}
}

uint8_t ScrollingCanvas::get(Coordinate<> c) const {
	return decorated_.get((c + offset_) % decorated_.size());
}

