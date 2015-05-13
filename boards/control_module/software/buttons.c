
#include "config.h"
#include "buttons.h"

void buttons_read(void) {
	static const uint8_t request[] = { 0, 1, 2, 3, 4, 5, 6, 7, 0 };
	
	spi_ss_activate_only(SPI_SS_PIN_BUTTONS);
	spi_readwrite(9, request, buttons_state);
	spi_ss_deactivate_all();
}

/**
 * Return state of button with given index.
 * @return 1 iff button is pressed.
 */
uint8_t buttons_get(uint8_t idx) {
	return !(buttons_state[idx / 8 + 1] & (1 << (idx % 8)));
}

