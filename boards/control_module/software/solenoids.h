
#ifndef SOLENOIDS_H
#define SOLENOIDS_H

#include "bit_actuator.h"

namespace solenoids_detail {
	enum class Index {
		FLIPPER_LEFT = 0,
		FLIPPER_RIGHT = 1,
		DTB0 = 2,
		SLINGSHOT0 = 3,
		SLINGSHOT1 = 4
	};
}

class Solenoids : public BitActuator<solenoids_detail::Index, Spi::Slave::SOLENOIDS, 2> {
	public:
		typedef BitActuator<solenoids_detail::Index, Spi::Slave::SOLENOIDS, 2> Super;

		Solenoids(Spi& spi) : Super(spi) { }
};

#endif // SOLENOIDS_H

