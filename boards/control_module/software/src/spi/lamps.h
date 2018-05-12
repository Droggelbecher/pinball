
#ifndef LAMPS_H
#define LAMPS_H

#include "spi.h"
#include "bit_actuator.h"

namespace pinball {

namespace lamps_detail {
	enum class Index {
		MAX = 64
	};
}

using Lamps = BitActuator<lamps_detail::Index, Spi::Slave::LAMPS>;

}

#endif // LAMPS_H

