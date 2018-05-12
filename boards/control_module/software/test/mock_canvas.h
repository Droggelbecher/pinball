#ifndef mock_canvas_h_INCLUDED
#define mock_canvas_h_INCLUDED

namespace pinball {
namespace test {

	class MockCanvas {
		private:

			const Coordinate<> size_ = { 3, 5 };

			uint8_t data[3 * 5] = {
				00, 01, 02, 03, 04,
				10, 11, 12, 13, 14,
				20, 21, 22, 23, 24
			};

		public:

			void set_pixel(Coordinate<> c, uint8_t color) {
			}

			uint8_t get_pixel(Coordinate<> c) const {
				return data[c.column() + size_.column() * c.row()];
			}

			uint8_t* buffer() {
				return static_cast<uint8_t*>(data);
			}

			size_t buffer_length() const {
				return size_.row() * size_.column();
			}

			Coordinate<> size() {
				return size_;
			}

	};

} } // ns pinball::test


#endif // mock_canvas_h_INCLUDED

