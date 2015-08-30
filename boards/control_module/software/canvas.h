
#ifndef CANVAS_H
#define CANVAS_H

#include <cstdint>

class Canvas {
	public:
		struct Coordinate {
			int row;
			int column;
		};
};

class ScrollingCanvas : public Canvas {

	public:

		ScrollingCanvas(const Canvas&, Coordinate);

		void next_frame();
		void resize(Coordinate);
};

#endif // CANVAS_H

