
#ifndef __CURSES_INTERFACE_H__
#define __CURSES_INTERFACE_H__

#include <bitset>

#include "canvas/canvas.h"
#include "canvas/canvas_buffer.h"

class CursesInterface : public Canvas {
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

		enum Index {
			FLIPPER_LEFT, FLIPPER_RIGHT,
			DTB0, DTB0_0, DTB0_1, DTB0_2, DTB0_3, DTB0_4,
			SLINGSHOT0, SLINGSHOT1,
			BUMPER0, BUMPER1, BUMPER2,
			BALL_RETURN, BALL_OUT, HOLE0,
			MAX
		};

		using SwitchesIndex = Index;
		using SolenoidsIndex = Index;
		using LampsIndex = Index;

		typedef std::bitset<MAX + 1> Bitset;

	public:

		CursesInterface(Coordinate<>);
		~CursesInterface();

		void next_frame() override;

		Coordinate<> canvas_size() const override { return buffer_.canvas_size(); }
		void set_pixel(Coordinate<> c, uint8_t v) override;
		uint8_t get_pixel(Coordinate<> c) const override;

		// Raw access to internal buffer

		int buffer_length() const { return buffer_.buffer_length(); }
		uint8_t* buffer() { return buffer_.buffer(); }

		bool get_switch(SwitchesIndex) const;
		const Bitset& get_switches_bits() const;

		struct Switches {
				using Index = SwitchesIndex;
				using Bitset = Bitset;
				Switches(CursesInterface *parent) : parent(parent) { };
				bool get(Index i) const { return parent->get_switch(i); }
				const Bitset& get_bits() const { return parent->get_switches_bits(); }
				void next_frame() const {}
			private:
				CursesInterface *parent;
		};
		Switches& switches() { return switches_; }

		void set_solenoid(SolenoidsIndex, bool);
		void set_lamp(LampsIndex, bool);

	private:
		CanvasBuffer buffer_;
		static const char color_symbols[];
		static const uint8_t color_fg[];
		static const uint8_t color_bg[];
		Switches switches_ { this };
};

#endif // __CURSES_INTERFACE_H__

