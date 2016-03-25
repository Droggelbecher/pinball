
#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "canvas/canvas.h"

class Display : public Canvas {
	public:

		Display();
		~Display();

		Coordinate<> size() const override { return Coordinate<>(0, 0); }

};

#endif // __DISPLAY_H__

