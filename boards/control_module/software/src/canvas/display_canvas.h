
#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "canvas.h"
#include "canvas_buffer.h"

class DisplayCanvas : public Canvas {
	public:
		DisplayCanvas(Coordinate<> size);
		Coordinate<> canvas_size() const override;

		void clear();
		void fill(uint8_t v);

	private:
		CanvasBuffer buffer_;
};

#endif // __DISPLAY_H__

