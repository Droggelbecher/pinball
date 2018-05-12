
#ifndef __SPI_DISPLAY_H__
#define __SPI_DISPLAY_H__

#include "canvas/canvas.h"
#include "spi/spi.h"
#include "coordinate.h"

namespace pinball {

class SpiDisplay {
	public:
		enum Color {
			C_BLACK = 0,
			C_RED = 1,
			C_GREEN = 2,
			C_YELLOW = 3,
			C_ORANGE = 4,
			C_DARK_RED = 5,
			C_DARK_GREEN = 6,
			C_BLOOD_ORANGE = 7,

			COLORS = 8,
		};

		SpiDisplay(Spi&, uint8_t, Coordinate<>);
		~SpiDisplay();

		Coordinate<> size() const;
		void next_frame(double dt);

		void clear();
		void set_pixel(Coordinate<> c, uint8_t color);
		uint8_t get_pixel(Coordinate<> c) const;

		int buffer_length() const;
		uint8_t* buffer();

	private:
		Spi& spi_;
		const uint8_t modules_;
		const Coordinate<> module_size_;
		uint8_t *display_screen_;
};

} // ns pinball

#if !MOCK_SPI
	#include "spi_display.cc"
#endif

#endif // __SPI_DISPLAY_H__

