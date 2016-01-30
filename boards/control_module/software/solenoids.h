
#ifndef SOLENOIDS_H
#define SOLENOIDS_H

#include "spi.h"

class Solenoids {

	public:
		enum class Index { FLIPPER_LEFT = 0, FLIPPER_RIGHT, DTB0 };

		//typedef std::bitset<16> Bitset;

		Solenoids(Spi& spi) : spi(spi) { }

		void next_frame();

		void set(Index, bool);

	private:
		//Bitset bits;
		Spi& spi;

		uint8_t state[3];
};

#if 0
#include <stdint.h>

#include <checksum.h>
#include <spi_protocols/solenoids.h>

#include "spi_ss.h"

uint8_t solenoids_state[3];

void solenoids_write(void);
void solenoids_set(uint8_t, uint8_t);
#endif

#endif // SOLENOIDS_H

