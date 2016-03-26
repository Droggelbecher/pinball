
#include "canvas_buffer.h"

CanvasBuffer::CanvasBuffer(Coordinate<> size)
	: size_(size),
	buffer_(new uint8_t[size_.row() * size_.column()])
{
}

uint8_t CanvasBuffer::get(Coordinate<> c) const {
	return buffer_[c.column() + c.row() * size_.column()];
}

void CanvasBuffer::set(Coordinate<> c, uint8_t color) {
	buffer_[c.column() + c.row() * size_.column()] = color;
}

