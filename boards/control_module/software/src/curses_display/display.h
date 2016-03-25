
#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "canvas/canvas.h"
#include "canvas/canvas_buffer.h"

class Display : public Canvas {
	public:

		Display(Coordinate<>);
		~Display();

		Coordinate<> size() const override { return Coordinate<>(0, 0); }

		void refresh();

		int buffer_length() const { return buffer_.buffer_length(); }
		uint8_t* buffer() { return buffer_.data(); }

	private:
		CanvasBuffer buffer_;

};

#endif // __DISPLAY_H__

