
#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "canvas/canvas.h"
#include "canvas/canvas_buffer.h"

class Display : public Canvas {
	public:

		Display(Coordinate<>);
		~Display();

		Coordinate<> size() const override { return buffer_.size(); }

		void next_frame() override;

		void set(Coordinate<> c, uint8_t color) override;
		uint8_t get(Coordinate<> c) const override;

		// Raw access to internal buffer

		int buffer_length() const { return buffer_.buffer_length(); }
		uint8_t* buffer() { return buffer_.buffer(); }

	private:
		CanvasBuffer buffer_;
};

#endif // __DISPLAY_H__

