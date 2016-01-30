
#ifndef SWITCHES_H
#define SWITCHES_H

#include <bitset>
#include "spi.h"

class Switches {

	public:
		enum {
			DATA_BYTES = 8,
			DATA_BITS = DATA_BYTES * 8
		};
		enum Index {
			FLIPPER_LEFT = 0,
			FLIPPER_RIGHT,
			DTB0_0,
			DTB0_1,
			DTB0_2,
			DTB0_3,
			DTB0_4
		};

		typedef std::bitset<DATA_BITS> Bitset;

		Switches(Spi& spi) : spi(spi) { }

		void next_frame();
		bool get(Index);
		const Bitset& get_bits() { return bits; }

	private:
		void read();

		Bitset bits;
		Spi& spi;
};

#endif // SWITCHES_H

