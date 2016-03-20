
#include "canvas_buffer.h"

CanvasBuffer::CanvasBuffer(Coordinate<> size)
	: size_(size),
	buffer_(new uint8_t[size_.row() * size_.column()])
{
}

