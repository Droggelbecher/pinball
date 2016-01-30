
#ifndef LAMPS_H
#define LAMPS_H

class Lamps {

};

#if 0
#include <stdint.h>

#include <checksum.h>
#include <spi_protocols/lamps.h>

#include "spi_ss.h"

uint8_t lamps_state[9];

void lamps_write(void);
void lamps_set(uint8_t idx, uint8_t v);
void lamps_clear(void);
#endif

#endif // LAMPS_H

