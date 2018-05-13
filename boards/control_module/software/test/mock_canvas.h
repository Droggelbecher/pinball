#ifndef mock_canvas_h_INCLUDED
#define mock_canvas_h_INCLUDED

#include <vector>

namespace pinball {
namespace test {

	class MockCanvas {
		public:

			MockCanvas(Coordinate<> size, std::initializer_list<uint8_t> l)
				: size_(size), data(l) {
			}

			void set_pixel(Coordinate<> c, uint8_t color) {
			}

			uint8_t get_pixel(Coordinate<> c) const {
				return data[c.column() + size_.column() * c.row()];
			}

			Coordinate<> size() const {
				return size_;
			}

			uint8_t* buffer() {
				return static_cast<uint8_t*>(data.data());
			}

			const uint8_t* buffer() const {
				return static_cast<const uint8_t*>(data.data());
			}

			size_t buffer_length() const {
				return size_.row() * size_.column();
			}

		private:
			Coordinate<> size_;
			std::vector<uint8_t> data;
	};

} } // ns pinball::test


#endif // mock_canvas_h_INCLUDED

