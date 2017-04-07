
#ifndef __CURSES_INTERFACE_H__
#define __CURSES_INTERFACE_H__

#include <bitset>

#include "canvas/canvas.h"
#include "canvas/canvas_buffer.h"
#include "memory_sensor_actuator.h"

/**
 *
 * TODO XXX
 *
 * So this is the idea:
 * 
 * abstract class Interface; // read: User Interface (not "abstract class", although it actually is)
 * 
 * class DummyInterface : Interface; // Simple dummy interface that just stores or discards in/output
 * class SpiInterface : Interface; // Actual interface to the physical pinball machine
 * class CursesInterface : Interface; // Decorator that adds a curses UI on top of any interface.
 */

/**
 * Interface wrapper that displays current state
 * using curses in the terminal and allows manipulation
 * using the keyboard.
 */
class CursesInterface {
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

	public:
		// TODO: Actually define SensorActuatorOverride
		using Switches = SensorActuatorOverride<SwitchesIndex>;
		using Solenoids = SensorActuatorOverride<SolenoidsIndex>;
		using Lamps = SensorActuatorOverride<LampsIndex>;

		CursesInterface(Coordinate<> display_size, DecoratedInterface& decorated);
		~CursesInterface();

		void next_frame() override;

		Coordinate<> canvas_size() const override { return buffer_.canvas_size(); }

		// TODO: delegate set_pixel to wrapped
		void set_pixel(Coordinate<> c, uint8_t v) override {
			buffer_.set_pixel(c, v);
			if(decorated_) {
				decorated_->set_pixel(c, v);
			}
		}

		// TODO: Is this method even being used?
		/*
		uint8_t get_pixel(Coordinate<> c) const override {
			if(decorated_) {
				return decorated_.get_pixel(c);
			}
		}
		*/

		// Raw access to internal canvas buffer
		// TODO: Either get rid of this entirely or re-use wrapped's buffer
		// if wrapped exists and only provide one if it doesn't
		//int buffer_length() const { return buffer_.buffer_length(); }
		//uint8_t* buffer() { return buffer_.buffer(); }

		// TODO:
		// If no wrapped: Just use keyboard-controlled memory switches
		// If wrapped: Disable keyboard or is there an obvious way to use both real switches + keyboard at the same time?
		//    possibly xor the state?
		//    Also: Which state should the switches display show (ideally what ever we decided above to actually be the state?)
		// TODO: This has to be done in instantiation of switches_, ie. in constructor
		Switches& switches() { return switches_; }

		// TODO:
		// If wrapped, forward to wrapped solenoids, also keep memory for internal displaying.
		Solenoids& solenoids() { return solenoids_; }

		// TODO
		Lamps& lamps() { return lamps_; }

	private:
		void print_state(const char *s, bool active, int row, int column);
		void handle_keys();

		CanvasBuffer buffer_;
		static const char color_symbols[];
		static const uint8_t color_fg[];
		static const uint8_t color_bg[];
		Switches switches_;
		Solenoids solenoids_;
		Lamps lamps_;
};

#endif // __CURSES_INTERFACE_H__

