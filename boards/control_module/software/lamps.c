
#include "config.h"
#include "lamps.h"
#include "spi.h"

void lamps_write(void) {
	lamps_state[8] = checksum(lamps_state, 8);

	spi_ss_activate_only(SPI_SS_PIN_LAMPS);

	/*int i;*/
	/*for(i = 0; i < 9; i++) {*/
		/*printf("lamps write [%d] = %02x\n", i, lamps_state[i]);*/
	/*}*/

	spi_readwrite(9, lamps_state, 0);
}

void lamps_set(uint8_t idx, uint8_t v) {
	if(v) {
		lamps_state[idx / 8] |= 1 << (idx % 8);
	}
	else {
		lamps_state[idx / 8] &= ~(1 << (idx % 8));
	}
}

void lamps_clear(void) {
	memset(lamps_state, 0x00, sizeof(lamps_state));
}


