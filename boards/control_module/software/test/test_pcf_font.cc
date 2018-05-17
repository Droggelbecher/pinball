
#include "coordinate.h"

#include "pcf_font.h"

#include "canvas/canvas.h"
#include "canvas/buffer.h"
#include "canvas/scrolling.h"

#include "mock_canvas.h"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE TestCanvas
#include <boost/test/unit_test.hpp>

namespace pinball {
	namespace test {

BOOST_AUTO_TEST_CASE(test_pcf_char_gohu) {
	using namespace canvas;

	PcfFont font { "../../resources/gohufont-11.pcf" };

	const auto& c = font.get_char('^');

	std::cout << "get_char('b') = " << c;
}

} } // ns pinball::test

