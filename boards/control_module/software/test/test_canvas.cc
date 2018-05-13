
#include "coordinate.h"

#include "canvas/canvas.h"
#include "canvas/canvas_buffer.h"
#include "canvas/scrolling.h"

#include "mock_canvas.h"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE TestCanvas
#include <boost/test/unit_test.hpp>

namespace pinball {
	namespace test {

BOOST_AUTO_TEST_CASE(test_blit_full) {
	/*
	 * Do a full blit from one canvas buffer to the other.
	 * Verify that the target canvas buffer has identical contents to the source afterwards.
	 */
	using namespace canvas;

	Coordinate<> sz(10, 5);

	CanvasBuffer a(sz);
	CanvasBuffer b(sz);
	clear(a);
	clear(b);

	a.set_pixel({0, 0}, 1);
	a.set_pixel({0, 1}, 1);
	a.set_pixel({0, 4}, 1);
	a.set_pixel({8, 2}, 1);
	a.set_pixel({2, 3}, 0);

	blit(a, b, {0, 0}, {10, 5}, {0, 0});

	for(int row = 0; row < sz.row(); row++) {
		for(int column = 0; column < sz.column(); column++) {
			BOOST_CHECK(
				a.get_pixel({row, column})
				== b.get_pixel({row, column})
			);
		} // for column
	} // for row
}

BOOST_AUTO_TEST_CASE(test_scrolling) {

	using namespace canvas;

	MockCanvas mock_canvas {
		{ 3, 5 }, {
		00, 01, 02, 03, 04,
		10, 11, 12, 13, 14,
		20, 21, 22, 23, 24
	} };


	Scrolling<MockCanvas> scrolling(mock_canvas, { 1.0, 0.0 });

	std::cout << "mock_canvas = " << mock_canvas;
	std::cout << "scrolling = " << scrolling;

	BOOST_CHECK(
		scrolling == mock_canvas
	);

	scrolling.next_frame(1.0);

	std::cout << "scrolling = " << scrolling;

	BOOST_CHECK(
		scrolling != mock_canvas
	);
}

} } // ns pinball::test

