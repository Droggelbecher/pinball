
#ifndef SOLENOIDS_H
#define SOLENOIDS_H

#include <stdint.h>

#include <checksum.h>
#include <spi_protocols/solenoids.h>

#include "spi_ss.h"

uint8_t solenoids_state[3];

void solenoids_write(void);
void solenoids_set(uint8_t, uint8_t);

#endif // SOLENOIDS_H

