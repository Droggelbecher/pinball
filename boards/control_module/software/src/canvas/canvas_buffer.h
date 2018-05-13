
#ifndef CANVAS_BUFFER_H
#define CANVAS_BUFFER_H

#include <memory>
#include <cstring>

#include "coordinate.h"

#include "canvas/canvas.h" // DataOrder

namespace pinball {
	namespace canvas {

/**
 *
 */
class CanvasBuffer {
	public:
		static const DataOrder data_order = COLUMN_FIRST;

		CanvasBuffer(Coordinate<> size);
		void set_pixel(Coordinate<> c, uint8_t color) ;
		uint8_t get_pixel(Coordinate<> c) const ;
		Coordinate<> size() const  { return size_; }

		int buffer_length() const { return size_.row() * size_.column(); }
		uint8_t* buffer() { return buffer_.get(); }
		const uint8_t* buffer() const { return buffer_.get(); }

	private:
		Coordinate<> size_;
		std::unique_ptr<uint8_t[]> buffer_;
};

} } // ns pinball::canvas

#endif // CANVAS_BUFFER_H

