
#include <cassert>
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
	assert(size_.column() > 0 && size_.row() > 0);
	memset(buffer_.get(), 0, buffer_length());
}

uint8_t Buffer::get_pixel(Coordinate<> c) const {
	assert(buffer_);
	assert(0 <= c.column() && c.column() < size_.column());
	assert(0 <= c.row() && c.row() < size_.row());

	return buffer_[c.column() + c.row() * size_.column()];
}

void Buffer::set_pixel(Coordinate<> c, uint8_t color) {
	assert(buffer_);
	assert(0 <= c.column() && c.column() < size_.column());
	assert(0 <= c.row() && c.row() < size_.row());

	buffer_[c.column() + c.row() * size_.column()] = color;
}

} } // ns pinball::canvas

