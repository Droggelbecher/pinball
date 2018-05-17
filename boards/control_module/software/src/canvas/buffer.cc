
#include "buffer.h"

namespace pinball {
	namespace canvas {

Buffer::Buffer()
	: size_(0, 0),
	buffer_(nullptr)
{
}

Buffer::Buffer(Coordinate<> size)
	: size_(size),
	buffer_(new uint8_t[size_.row() * size_.column()])
{
	memset(buffer_.get(), 0, buffer_length());
}

uint8_t Buffer::get_pixel(Coordinate<> c) const {
	return buffer_[c.column() + c.row() * size_.column()];
}

void Buffer::set_pixel(Coordinate<> c, uint8_t color) {
	buffer_[c.column() + c.row() * size_.column()] = color;
}

} } // ns pinball::canvas

