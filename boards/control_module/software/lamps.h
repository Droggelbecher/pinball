
#ifndef LAMPS_H
#define LAMPS_H

#include "bit_actuator.h"

namespace lamps_detail {
	enum class Index {
	};
}

class Lamps : public BitActuator<lamps_detail::Index, Spi::Slave::LAMPS, 8> {
	public:
		typedef BitActuator<lamps_detail::Index, Spi::Slave::LAMPS, 8> Super;

		Lamps(Spi& spi) : Super(spi) { }
};

#endif // LAMPS_H

