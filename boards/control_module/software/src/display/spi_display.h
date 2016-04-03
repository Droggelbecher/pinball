
#ifndef __SPI_DISPLAY_H__
#define __SPI_DISPLAY_H__

#include "canvas/canvas.h"
#include "spi.h"
#include "coordinate.h"

class SpiDisplay : public Canvas {
	public:
		enum {
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

		void set(Coordinate<> c, uint8_t color) override;
		uint8_t get(Coordinate<> c) const override;

		int buffer_length() const;
		uint8_t* buffer();

	private:
		Spi& spi_;
		uint8_t modules_;
		Coordinate<> module_size_;
		uint8_t *display_screen_;
};

/*
enum {
	DISPLAY_SCREEN_BYTES =
		DISPLAY_MODULE_COUNT * DISPLAY_MODULE_ROWS *
		DISPLAY_MODULE_COLUMNS,
};

#define DISPLAY_PHASE(T, n, frame) ((frame % (unsigned long)((T) * DISPLAY_TARGET_FPS)) / (unsigned long)((T) * DISPLAY_TARGET_FPS / (n)))

extern unsigned char display_screen_[DISPLAY_SCREEN_BYTES];
extern unsigned long display_frame;

void display_setup(void);

void display_start_frame(void);
void display_end_frame(void);
void display_refresh(void);
void display_debug_fps(void);

uint8_t display_sane(uint8_t row, uint8_t column);
unsigned char* display_screen(uint8_t row, uint8_t column);
*/

#endif // __SPI_DISPLAY_H__

