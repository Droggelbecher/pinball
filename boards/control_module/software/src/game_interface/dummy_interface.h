#ifndef game_interface__dummy_interface_h_INCLUDED
#define game_interface__dummy_interface_h_INCLUDED

#include "memory_sensor_actuator.h"
#include "coordinate.h"

namespace pinball {

class DummyInterface {
		struct DummyCanvas {
			void clear() { };
			void set_pixel(Coordinate<> c, uint8_t co) { };
			uint8_t get_pixel(Coordinate<> c) const { return 0; }
			Coordinate<> size() const { return { 0, 0 }; }
		};

	public:
		enum class SwitchesIndex {
			FLIPPER_LEFT = 2,
			FLIPPER_RIGHT = 5,
			DTB0_0 = 8,
			DTB0_1 = 9,
			DTB0_2 = 10,
			DTB0_3 = 11,
			DTB0_4 = 12,

			SLINGSHOT0 = 3,
			SLINGSHOT1 = 4,

			BUMPER0 = 13,
			BUMPER1 = 14,
			BUMPER2 = 15,

			BALL_OUT = 22,
			HOLE0 = 23,
			MAX
		};

		enum class LampsIndex {
			// TODO
			MAX = 64
		};

		enum class SolenoidsIndex {
			FLIPPER_LEFT = 0,
			FLIPPER_RIGHT = 1,
			DTB0 = 2,
			SLINGSHOT0 = 3,
			SLINGSHOT1 = 4,
			BUMPER0 = 5,//SPI_SOLENOIDS_BUMPER_0_IDX,
			BUMPER1 = 6,//SPI_SOLENOIDS_BUMPER_1_IDX,
			BUMPER2 = 7,//SPI_SOLENOIDS_BUMPER_2_IDX

			BALL_RETURN = 8,
			AUX0 = 9,
			AUX1 = 10,
			MAX
		};

		using Canvas = DummyCanvas;
		using Switches = MemorySensorActuator<SwitchesIndex>;
		using Solenoids = MemorySensorActuator<SolenoidsIndex>;
		using Lamps = MemorySensorActuator<LampsIndex>;

		void next_frame(double dt) { }

		Canvas& canvas() { return canvas_; }

		Switches& switches() { return switches_; }
		Solenoids& solenoids() { return solenoids_; }
		Lamps& lamps() { return lamps_; }

	private:

		DummyCanvas canvas_;
		Switches switches_;
		Solenoids solenoids_;
		Lamps lamps_;
};


template<typename C>
void blit(
	const C& source, DummyInterface::DummyCanvas& target,
	Coordinate<>, Coordinate<>, Coordinate<>
) {
}

} // ns pinball

#endif // game_interface__dummy_interface_h_INCLUDED

