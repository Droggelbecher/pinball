
#ifndef LAMPS_H
#define LAMPS_H

#include "spi.h"
#include "bit_actuator.h"

namespace lamps_detail {
	enum class Index {
		MAX = 64
	};
}

using Lamps = BitActuator<lamps_detail::Index, Spi::Slave::LAMPS>;
/*
class Lamps : public BitActuator<lamps_detail::Index, Spi::Slave::LAMPS> {
	public:
		using Super = BitActuator<lamps_detail::Index, Spi::Slave::LAMPS>;

		Lamps(Spi& spi) : Super(spi) { }
};
*/
#endif // LAMPS_H

