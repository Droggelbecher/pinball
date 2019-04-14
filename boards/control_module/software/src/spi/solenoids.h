
#ifndef SOLENOIDS_H
#define SOLENOIDS_H

#include "spi.h"
#include "bit_actuator.h"

#include <spi_protocols/solenoids.h>

namespace pinball {

namespace solenoids_detail {
	enum class Index {
		FLIPPER_LEFT = SPI_SOLENOIDS_FLIPPER_LEFT_IDX,
		FLIPPER_RIGHT = SPI_SOLENOIDS_FLIPPER_RIGHT_IDX,
		DTB0 = SPI_SOLENOIDS_DROP_TARGET_BANK_0_IDX,
		SLINGSHOT0 = SPI_SOLENOIDS_SLINGSHOT_LEFT_IDX,
		SLINGSHOT1 = SPI_SOLENOIDS_SLINGSHOT_RIGHT_IDX,
		BUMPER0 = SPI_SOLENOIDS_BUMPER_0_IDX,
		BUMPER1 = SPI_SOLENOIDS_BUMPER_1_IDX,
		BUMPER2 = SPI_SOLENOIDS_BUMPER_2_IDX,

		// Hardware problems currently in both port BALL_RETURN and AUX_1,
		// luckily we currently have 2 spares anyway, use AUX_0 for ball return
		AUX0 = SPI_SOLENOIDS_BALL_RETURN_IDX,
		BALL_RETURN = SPI_SOLENOIDS_AUX_0_IDX,

		AUX1 = SPI_SOLENOIDS_AUX_1_IDX,

		MAX = 16
	};
} // ns solenoids_detail

using Solenoids = BitActuator<solenoids_detail::Index, Spi::Slave::SOLENOIDS>;

} // ns pinball

#endif // SOLENOIDS_H

