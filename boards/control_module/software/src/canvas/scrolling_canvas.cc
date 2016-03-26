
#include "scrolling_canvas.h"

ScrollingCanvas::ScrollingCanvas(Canvas& decorated, Coordinate<> speed)
	: decorated_(decorated), speed_(speed)
{
}

void ScrollingCanvas::next_frame() {
	offset_ += speed_;
	offset_ %= decorated_.size();
}

void ScrollingCanvas::resize(Coordinate<> new_size) {
	decorated_.resize(new_size);
	offset_ %= decorated_.size();
}

void ScrollingCanvas::set(Coordinate<> c, uint8_t color) {
	decorated_.set((c + offset_) % decorated_.size(), color);
}

uint8_t ScrollingCanvas::get(Coordinate<> c) const {
	return decorated_.get((c + offset_) % decorated_.size());
}

