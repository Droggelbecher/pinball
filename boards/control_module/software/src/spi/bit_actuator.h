
#ifndef BIT_ACTUATOR_H
#define BIT_ACTUATOR_H

#include "spi.h"

template<
	typename Index_,
	Spi::Slave SLAVE_
>
class BitActuator {
	public:
		static const Spi::Slave SLAVE = SLAVE_;

		using Index = Index_;
		enum {
			DATA_BYTES = (static_cast<int>(Index::MAX) + 7) / 8,
			DATA_BITS = DATA_BYTES * 8
		};

		BitActuator(Spi& spi) : spi(spi) {
			for(int i = 0; i < DATA_BYTES + 1; i++) {
				state[i] = 0xff;
			}
		}

		void next_frame();
		void set(Index, bool);
		bool get(Index) const;

	private:
		Spi& spi;
		uint8_t state[DATA_BYTES + 1];

};

#include "bit_actuator.cc"

#endif

