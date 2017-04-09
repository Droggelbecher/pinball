
#ifndef CANVAS_BUFFER_H
#define CANVAS_BUFFER_H

#include <memory>
#include <cstring>

#include "coordinate.h"
#include "canvas/canvas.h"

/**
 *
 */
class CanvasBuffer : public Canvas {
	public:
		CanvasBuffer(Coordinate<> size);

		void set_all(uint8_t c) {
			memset(buffer_.get(), c, size_.row() * size_.column());
		}

		void set_pixel(Coordinate<> c, uint8_t color) override;
		uint8_t get_pixel(Coordinate<> c) const override;

		Coordinate<> size() const override { return size_; }

		// Raw access

		int buffer_length() const { return size_.row() * size_.column(); }
		uint8_t* buffer() { return buffer_.get(); }

	private:
		Coordinate<> size_;
		std::unique_ptr<uint8_t[]> buffer_;
};


#endif // CANVAS_BUFFER_H

