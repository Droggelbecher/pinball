
#ifndef __CURSES_INTERFACE_H__
#define __CURSES_INTERFACE_H__

#include "canvas/canvas.h"
#include "canvas/canvas_buffer.h"

class CursesInterface : public Canvas {
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

	private:
		static const char color_symbols[];
		static const uint8_t color_fg[];
		static const uint8_t color_bg[];

	public:

		CursesInterface(Coordinate<>);
		~CursesInterface();

		Coordinate<> size() const override { return buffer_.size(); }

		void next_frame() override;

		void set(Coordinate<> c, uint8_t color) override;
		uint8_t get(Coordinate<> c) const override;

		// Raw access to internal buffer

		int buffer_length() const { return buffer_.buffer_length(); }
		uint8_t* buffer() { return buffer_.buffer(); }

	private:
		CanvasBuffer buffer_;
};

#endif // __CURSES_INTERFACE_H__

