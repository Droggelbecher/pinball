
#include <assert.h>
#include <string.h> // memset
#include <checksum.h>

#include "config.h"
#include "switches.h"

void switches_read(void) {
	static uint8_t request[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	uint8_t answer[9];
	memset(answer, 0x00, sizeof(answer));
	
	spi_ss_activate_only(SPI_SS_PIN_SWITCHES);
	spi_readwrite(sizeof(answer), request, answer);

	if(checksum(answer, sizeof(answer) - 1) == answer[sizeof(answer) - 1]) {
		assert(sizeof(answer) == sizeof(switches_state_));
		memcpy(switches_state_, answer, sizeof(switches_state_));
	}

	spi_ss_deactivate_all();
}

uint8_t switches_get(uint8_t idx) {
	return !(switches_state_[idx / 8 + 1] & (1 << (idx % 8)));
}

uint8_t switches_get_range(uint8_t idx, uint8_t len) {
	const uint8_t mask = (1 << len) - 1;
	return (switches_state_[idx / 8 + 1] & (mask << (idx % 8))) ^ mask;
}


