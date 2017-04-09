
#ifndef __SPI_DISPLAY_H__
#define __SPI_DISPLAY_H__

#include "canvas/canvas.h"
#include "spi/spi.h"
#include "coordinate.h"

class SpiDisplay : public Canvas {
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

		Coordinate<> size() const override;
		void next_frame() override;

		void set_pixel(Coordinate<> c, uint8_t color) override;
		uint8_t get_pixel(Coordinate<> c) const override;

		int buffer_length() const;
		uint8_t* buffer();

	private:
		Spi& spi_;
		uint8_t modules_;
		Coordinate<> module_size_;
		uint8_t *display_screen_;
};

#if !MOCK_SPI
	#include "spi_display.cc"
#endif

#endif // __SPI_DISPLAY_H__

