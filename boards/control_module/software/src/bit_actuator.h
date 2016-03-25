
#ifndef BIT_ACTUATOR_H
#define BIT_ACTUATOR_H

#include "spi.h"

template<
	typename Index_,
	Spi::Slave SLAVE_,
	int N
>
class BitActuator {
	public:
		static const Spi::Slave SLAVE = SLAVE_;
		using Index = Index_;

		BitActuator(Spi& spi) : spi(spi) { }

		void next_frame();
		void set(Index, bool);

	private:
		Spi& spi;
		uint8_t state[N + 1];

};

#include "bit_actuator.cc"

#endif

