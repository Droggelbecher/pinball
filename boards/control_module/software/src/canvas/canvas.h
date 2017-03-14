
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

		virtual void set_pixel(Coordinate<> c, uint8_t color) = 0;
		virtual uint8_t get_pixel(Coordinate<> c) const = 0;

		virtual void next_frame() { }
		
		virtual void resize(Coordinate<>) { }
		virtual Coordinate<> canvas_size() const = 0;
};

#endif // CANVAS_H

