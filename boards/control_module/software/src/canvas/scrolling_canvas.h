
#ifndef SCROLLING_CANVAS_H
#define SCROLLING_CANVAS_H

#include "canvas.h"

/**
 *
 */
class ScrollingCanvas : public Canvas {
	public:
		ScrollingCanvas(Canvas&, Coordinate<double>);

		void next_frame() override;

		void set(Coordinate<> c, uint8_t color) override;
		uint8_t get(Coordinate<> c) const override;

		void resize(Coordinate<>) override;
		Coordinate<> size() const override { return size_; }

	private:
		Canvas& decorated_;
		Coordinate<> size_;
		Coordinate<double> speed_;
		Coordinate<double> offset_;
};

#endif // SCROLLING_CANVAS_H