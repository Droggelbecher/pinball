
#include "config.h"
#include "solenoids.h"

void solenoids_write(void) {
	spi_ss_activate_only(SPI_SS_PIN_SOLENOIDS);

	spi_readwrite(2, solenoids_state, 0);
}

void solenoids_set(uint8_t idx, uint8_t v) {
	/*assert(idx >= 0);*/
	/*assert(idx <= 15);*/
	/*assert(v == 0 || v == 1);*/

	if(v) {
		solenoids_state[idx / 8] &= ~(1 << idx);
	}
	else {
		solenoids_state[idx / 8] |= 1 << idx;
	}
}


