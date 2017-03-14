
#include "broadcast_canvas.h"


BroadcastCanvas::BroadcastCanvas(std::unique_ptr<Canvas> c) {
	canvas_.push_back(std::move(c));
	//for(auto&& c: cs) {
		//canvas_.push_back(c);
	//}
}

void BroadcastCanvas::set_pixel(Coordinate<> c, uint8_t color) {
	for(auto& canvas: canvas_) {
		canvas->set_pixel(c, color);
	}
}

uint8_t BroadcastCanvas::get_pixel(Coordinate<> c) const {
	return canvas_[0]->get_pixel(c);
}

void BroadcastCanvas::next_frame() {
	for(auto& canvas: canvas_) {
		canvas->next_frame();
	}
}

Coordinate<> BroadcastCanvas::canvas_size() const {
	return canvas_[0]->canvas_size();
}

