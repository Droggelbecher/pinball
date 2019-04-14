
#ifndef BUFFER_H
#define BUFFER_H

#include <memory>
#include <cstring>

#include "coordinate.h"
#include "canvas/canvas.h" // DataOrder

namespace pinball {
	namespace canvas {

/**
 *
 */
class Buffer {
	public:
		static const DataOrder data_order = COLUMN_FIRST;

		Buffer();
		Buffer(Coordinate<> size);

		Buffer(Buffer&& other) {
			size_ = other.size_;
			buffer_ = std::move(other.buffer_);
			other.size_ = { 0, 0 };
		}

		Buffer& operator=(Buffer&& other) {
			size_ = other.size_;
			buffer_ = std::move(other.buffer_);
			other.size_ = { 0, 0 };
			return *this;
		}

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

#endif // BUFFER_H

