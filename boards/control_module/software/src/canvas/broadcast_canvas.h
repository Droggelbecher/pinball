
#ifndef BROADCAST_CANVAS_H
#define BROADCAST_CANVAS_H

#include <vector>
#include <memory>
#include <functional>

#include "canvas/canvas.h"

class BroadcastCanvas : public Canvas {
	public:
		BroadcastCanvas(std::initializer_list<std::reference_wrapper<Canvas> > cs);

		void set_pixel(Coordinate<> c, uint8_t color) override;
		uint8_t get_pixel(Coordinate<> c) const override;
		void next_frame(double dt) override;
		Coordinate<> size() const override;

	private:
		std::vector<std::reference_wrapper<Canvas> > canvas_;

};

#endif // BROADCAST_CANVAS_H

