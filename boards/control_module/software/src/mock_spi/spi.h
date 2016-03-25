
#ifndef __SPI_H__
#define __SPI_H__

#include <vector>
#include <stdint.h>

class Spi {
	public:

		enum {
			SPI_BUS_SPEED = 1000 * 1000,
			SPI_MODE = 0,
			SPI_BITS_PER_WORD = 8
		};

		enum Slave {
			LAMPS = 22,
			SWITCHES = 23,
			DISPLAY = 24,
			SOLENOIDS = 25,

			ALL_SLAVES = (1 << LAMPS)
				| (1 << DISPLAY)
				| (1 << SWITCHES)
				| (1 << SOLENOIDS)
		};

		Spi();

		std::vector<uint8_t> transfer_and_check(Slave slave, std::vector<uint8_t> input);
};


#endif // __SPI_H__

