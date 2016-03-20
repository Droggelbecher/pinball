
#ifndef SOLENOIDS_H
#define SOLENOIDS_H

#include "bit_actuator.h"

//#include <spi_protocols/solenoids.h>

namespace solenoids_detail {
	enum class Index {
		FLIPPER_LEFT = 0,
		FLIPPER_RIGHT = 1,
		DTB0 = 2,
		SLINGSHOT0 = 3,
		SLINGSHOT1 = 4,
		BUMPER0 = 5,//SPI_SOLENOIDS_BUMPER_0_IDX,
		BUMPER1 = 6,//SPI_SOLENOIDS_BUMPER_1_IDX,
		BUMPER2 = 7//SPI_SOLENOIDS_BUMPER_2_IDX

	};
}

class Solenoids : public BitActuator<solenoids_detail::Index, Spi::Slave::SOLENOIDS, 2> {
	public:
		typedef BitActuator<solenoids_detail::Index, Spi::Slave::SOLENOIDS, 2> Super;

		Solenoids(Spi& spi) : Super(spi) { }
};

#endif // SOLENOIDS_H

