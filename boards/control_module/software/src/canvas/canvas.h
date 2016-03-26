
#ifndef CANVAS_H
#define CANVAS_H

#include <cstdint>

#include "coordinate.h"

/**
 *
 */
class Canvas {
	public:
		virtual ~Canvas() { }

		virtual void set(Coordinate<> c, uint8_t color) = 0;
		virtual uint8_t get(Coordinate<> c) const = 0;

		virtual void next_frame() { }
		
		virtual void resize(Coordinate<>) { }
		virtual Coordinate<> size() const = 0;
};

#endif // CANVAS_H

