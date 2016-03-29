
#include "broadcast_canvas.h"


BroadcastCanvas::BroadcastCanvas(std::unique_ptr<Canvas> c) {
	canvas_.push_back(std::move(c));
	//for(auto&& c: cs) {
		//canvas_.push_back(c);
	//}
}

void BroadcastCanvas::set(Coordinate<> c, uint8_t color) {
	for(auto& canvas: canvas_) {
		canvas->set(c, color);
	}
}

uint8_t BroadcastCanvas::get(Coordinate<> c) const {
	return canvas_[0]->get(c);
}

void BroadcastCanvas::next_frame() {
	for(auto& canvas: canvas_) {
		canvas->next_frame();
	}
}

Coordinate<> BroadcastCanvas::size() const {
	return canvas_[0]->size();
}

