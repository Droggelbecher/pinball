
#include "broadcast_canvas.h"


BroadcastCanvas::BroadcastCanvas(std::initializer_list<std::reference_wrapper<Canvas> > cs) {
	for(auto c: cs) {
		canvas_.push_back(c);
	}
}

void BroadcastCanvas::set_pixel(Coordinate<> c, uint8_t color) {
	for(Canvas& canvas: canvas_) {
		canvas.set_pixel(c, color);
	}
}

uint8_t BroadcastCanvas::get_pixel(Coordinate<> c) const {
	return canvas_[0].get().get_pixel(c);
}

void BroadcastCanvas::next_frame(double dt) {
	for(Canvas& canvas: canvas_) {
		canvas.next_frame(dt);
	}
}

Coordinate<> BroadcastCanvas::size() const {
	return canvas_[0].get().size();
}

