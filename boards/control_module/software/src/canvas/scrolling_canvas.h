
#ifndef SCROLLING_CANVAS_H
#define SCROLLING_CANVAS_H

#include "canvas.h"

/**
 *
 */
class ScrollingCanvas : public Canvas {
	public:
		ScrollingCanvas(Canvas&, Coordinate<>);

		void next_frame() override;
		void resize(Coordinate<>) override;
		Coordinate<> size() const override { return decorated_.size(); }

	private:
		Canvas& decorated_;
		Coordinate<> speed_;
		Coordinate<> offset_;
};

#endif // SCROLLING_CANVAS_H
