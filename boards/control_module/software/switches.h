
#ifndef SWITCHES_H
#define SWITCHES_H

#include <stdint.h>

#include <spi_protocols/switches.h>

#include "spi.h"
#include "spi_ss.h"

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

#endif // SWITCHES_H

