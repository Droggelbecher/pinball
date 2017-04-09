
#ifndef BROADCAST_CANVAS_H
#define BROADCAST_CANVAS_H

#include <vector>
#include <memory>
#include <functional>

#include "canvas/canvas.h"

class BroadcastCanvas : public Canvas {
	public:
		BroadcastCanvas(std::initializer_list<std::reference_wrapper<Canvas> > cs);
		//BroadcastCanvas(std::unique_ptr<Canvas>);

		void set_pixel(Coordinate<> c, uint8_t color) override;
		uint8_t get_pixel(Coordinate<> c) const override;
		void next_frame() override;
		//void resize(Coordinate<>) override;
		Coordinate<> size() const override;

		// TODO: implement!
		//uint8_t *buffer() { return canvas_[0]->buffer(); }
		//int buffer_length() { return canvas_[0]->buffer_length(); }

	private:
		std::vector<std::reference_wrapper<Canvas> > canvas_;

};

#endif // BROADCAST_CANVAS_H

