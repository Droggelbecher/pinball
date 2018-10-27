
#ifndef __CURSES_INTERFACE_H__
#define __CURSES_INTERFACE_H__

#include <bitset>

#include "canvas/buffer.h"
//#include "canvas/broadcast_canvas.h"
#include "sensor_actuator_override.h"
#include "buffer_logger.h"

namespace pinball { namespace canvas {
	template<typename... Cs>
	class BroadcastCanvas;

} }


namespace pinball {

/**
 * Interface wrapper that displays current state
 * using curses in the terminal and allows manipulation
 * using the keyboard.
 */
template<typename TDecorated>
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

	public:
		using Canvas = canvas::BroadcastCanvas<canvas::Buffer, typename TDecorated::Canvas>;
		using Switches = SensorActuatorOverride<typename TDecorated::Switches>;
		using Solenoids = SensorActuatorOverride<typename TDecorated::Solenoids>;
		using Lamps = SensorActuatorOverride<typename TDecorated::Lamps>;
		using Logger = BufferLogger<10>;

		CursesInterface(Coordinate<> display_size, TDecorated& decorated);
		~CursesInterface();

		void next_frame(double dt);

		TDecorated& decorated() { return decorated_; }
		Switches& switches() { return switches_; }
		Solenoids& solenoids() { return solenoids_; }
		Lamps& lamps() { return lamps_; }
		Canvas& canvas() { return canvas_; }
		Logger& logger() { return logger_; }

	private:
		void print_state(const char *s, bool active, int row, int column);
		void handle_keys();

		static const char color_symbols[];
		static const uint8_t color_fg[];
		static const uint8_t color_bg[];

		TDecorated &decorated_;
		canvas::Buffer internal_canvas_;
		Canvas canvas_;
		Switches switches_;
		Lamps lamps_;
		Solenoids solenoids_;
		Logger logger_;
};

} // ns pinball

#include "game_interface/curses_interface.cc"

#endif // __CURSES_INTERFACE_H__

