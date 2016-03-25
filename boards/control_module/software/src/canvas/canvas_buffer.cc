
#include "canvas_buffer.h"

CanvasBuffer::CanvasBuffer(Coordinate<> size)
	: size_(size),
	buffer_(new uint8_t[size_.row() * size_.column()])
{
}

uint8_t CanvasBuffer::get(int row, int column) {
	return buffer_[column + row * size_.column()];
}

