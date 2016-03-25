
#include "pcf.h"
#include "test.h"

TEST(PcfTest) {
	Pcf pcf { "resources/gohufont-11.pcf" };

	DynamicBufferCanvas canvas;
	pcf.render_text(canvas, 0, 0, "Hello, World!");

	assert(canvas.size() == Coordinate(30, 8));
	
}

