
#ifndef CANVAS_H
#define CANVAS_H

#include <cstdint>

#include "coordinate.h"

/**
 *
 */
class Canvas {
	public:
		virtual void next_frame() { }
		virtual void resize(Coordinate<>) { }
		virtual Coordinate<> size() const = 0;
};

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

#endif // CANVAS_H

