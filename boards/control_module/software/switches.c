
#include "config.h"
#include "switches.h"

void switches_read(void) {
	static uint8_t request[] = { 0, 1, 2, 3, 4, 5, 6, 7, 0 };
	
	spi_ss_activate_only(SPI_SS_PIN_SWITCHES);
	spi_readwrite(9, request, switches_state);
	spi_ss_deactivate_all();
}

uint8_t switches_get(uint8_t idx) {
	return !(switches_state[idx / 8 + 1] & (1 << (idx % 8)));
}

uint8_t switches_get_range(uint8_t idx, uint8_t len) {
	const uint8_t mask = (1 << len) - 1;
	return (switches_state[idx / 8 + 1] & (mask << (idx % 8))) ^ mask;
}


