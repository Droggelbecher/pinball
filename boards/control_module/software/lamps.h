
#ifndef LAMPS_H
#define LAMPS_H

#include "bit_actuator.h"

namespace lamps_detail {
	enum class Index {
	};
}

class Solenoids : public BitActuator<solenoids_detail::Index, Spi::Slave::SOLENOIDS, 8> {
	public:
		typedef BitActuator<solenoids_detail::Index, Spi::Slave::SOLENOIDS, 8> Super;

		Lamps(Spi& spi) : Super(spi) { }
};

#endif // LAMPS_H

