
#ifndef SWITCHES_H
#define SWITCHES_H

#include <bitset>

namespace pinball {

class Spi;

class Switches {
	public:
		enum class Index {
			// Bank 0: 0..7
			FLIPPER_LEFT = 2,
			FLIPPER_RIGHT = 5,
			SLINGSHOT0 = 3,
			SLINGSHOT1 = 4,

			// Bank 1: 8..15
			DTB0_0 = 8,
			DTB0_1 = 9,
			DTB0_2 = 10,
			// yes the contacts for these are actually swapped,
			// less work to just correct it here in software :-)
			DTB0_4 = 11,
			DTB0_3 = 12,

			// Bank 2: 16..23
			BUMPER0 = 13,
			BUMPER1 = 14,
			BUMPER2 = 15,
			BALL_OUT = 22,
			HOLE0 = 23,

			MAX = 64
		};

		enum {
			DATA_BYTES = static_cast<int>(Index::MAX) / 8,
			DATA_BITS = DATA_BYTES * 8
		};

		typedef std::bitset<DATA_BITS> Bitset;

		Switches(Spi& spi);

		void next_frame(double dt);
		bool get(Index);
		const Bitset& get_bits() { return bits; }

		void set(bool) { }
		void set(Index, bool) { }


	private:
		void read();

		Bitset bits;
		Spi& spi;
};

} // ns pinball

#endif // SWITCHES_H

