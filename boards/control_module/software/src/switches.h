
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
			FLIPPER_LEFT = 2,
			FLIPPER_RIGHT = 5,
			DTB0_0 = 8,
			DTB0_1 = 9,
			DTB0_2 = 10,
			DTB0_3 = 11,
			DTB0_4 = 12,

			SLINGSHOT0 = 3,
			SLINGSHOT1 = 4,

			BUMPER0 = 13, // ?
			BUMPER1 = 14,
			BUMPER2 = 15,

			HOLE0 = 23,
			BALL_OUT = 22
		};

		typedef std::bitset<DATA_BITS> Bitset;

		Switches(Spi& spi);

		void next_frame();
		bool get(Index);
		const Bitset& get_bits() { return bits; }

	private:
		void read();

		Bitset bits;
		Spi& spi;
};

#endif // SWITCHES_H

