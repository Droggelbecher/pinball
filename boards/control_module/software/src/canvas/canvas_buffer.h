
#ifndef CANVAS_BUFFER_H
#define CANVAS_BUFFER_H

#include <memory>
#include <cstring>

#include "coordinate.h"

/**
 *
 */
class CanvasBuffer {
	public:
		CanvasBuffer(Coordinate<> size);

		void set_all(uint8_t c) { memset(buffer_.get(), c, size_.row() * size_.column()); }

		Coordinate<> size() const { return size_; }
		uint8_t get(int row, int column);

		int buffer_length() const { return size_.row() * size_.column(); }
		uint8_t* data() { return buffer_.get(); }

	private:
		Coordinate<> size_;
		std::unique_ptr<uint8_t[]> buffer_;
};


#endif // CANVAS_BUFFER_H

