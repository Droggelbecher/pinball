
#ifndef SWITCHES_H
#define SWITCHES_H


class Switches {

	public:
		enum class Index { FLIPPER_LEFT = 0, FLIPPER_RIGHT, DTB0 };

		bool get(Index);

		bool all(Index, unsigned);
};


//#include <stdint.h>

//#include <spi_protocols/switches.h>

//#include "spi.h"
//#include "spi_ss.h"


#if 0
uint8_t switches_state_[8];

/**
 * Read switches states from SPI.
 */
void switches_read(void);

/**
 * Return state of switch with given index.
 * @return 1 iff switch is pressed.
 */
uint8_t switches_get(uint8_t idx);


/**
 * Return state of switch with given index.
 * @return 1 iff switch is pressed.
 */
uint8_t switches_get_range(uint8_t idx, uint8_t len);
#endif // 0

#endif // SWITCHES_H

