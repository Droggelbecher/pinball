
#ifndef BUTTONS_H
#define BUTTONS_H

#include <stdint.h>

#include <spi_protocols/buttons.h>
//#define BUTTONS_FLIPPER_LEFT_IDX   2
//#define BUTTONS_FLIPPER_RIGHT_IDX  5

uint8_t buttons_state[9];

void buttons_read(void);

/**
 * Return state of button with given index.
 * @return 1 iff button is pressed.
 */
uint8_t buttons_get(uint8_t idx);


/**
 * Return state of button with given index.
 * @return 1 iff button is pressed.
 */
uint8_t buttons_get_range(uint8_t idx, uint8_t len);

#endif // BUTTONS_H

