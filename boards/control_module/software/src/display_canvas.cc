
#include "display_canvas.h"

DisplayCanvas::DisplayCanvas(Coordinate<> size) :
	buffer_(size)
{
}

void DisplayCanvas::clear() {
	buffer_.set_all(0);
}

void DisplayCanvas::fill(uint8_t v) {
	buffer_.set_all(v);
}

Coordinate<> DisplayCanvas::size() const {
	return buffer_.size();
}

