
#include "config.h"
#include "solenoids.h"
#include "spi.h"

void solenoids_write(void) {
	solenoids_state[2] = checksum(solenoids_state, 2);

	spi_ss_activate_only(SPI_SS_PIN_SOLENOIDS);

	spi_readwrite(3, solenoids_state, 0);
}

void solenoids_set(uint8_t idx, uint8_t v) {
	if(v) {
		solenoids_state[idx / 8] &= ~(1 << idx);
	}
	else {
		solenoids_state[idx / 8] |= 1 << idx;
	}
}


