
#ifndef SOLENOIDS_H
#define SOLENOIDS_H

#include "spi.h"
#include "bit_actuator.h"

//#include <spi_protocols/solenoids.h>

namespace solenoids_detail {
	enum class Index {
		FLIPPER_LEFT = 0,
		FLIPPER_RIGHT = 1,
		DTB0 = 2,
		SLINGSHOT0 = 3,
		SLINGSHOT1 = 4,
		BUMPER0 = 5,
		BUMPER1 = 6,
		BUMPER2 = 7,

		BALL_RETURN = 8,
		MAX = 16
	};
}

/*
class Solenoids : public BitActuator<solenoids_detail::Index, Spi::Slave::SOLENOIDS> {
	public:
		using Super = BitActuator<solenoids_detail::Index, Spi::Slave::SOLENOIDS>;

		Solenoids(Spi& spi) : Super(spi) { }
};
*/
using Solenoids = BitActuator<solenoids_detail::Index, Spi::Slave::SOLENOIDS>;

#endif // SOLENOIDS_H

