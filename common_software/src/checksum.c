
#include "checksum.h"

/*
 * A variant of CCIT CRC8
 */
uint8_t checksum(uint8_t* data, uint8_t len) {
	uint8_t mask = 0xe0; // source: http://en.wikipedia.org/wiki/Polynomial_representations_of_cyclic_redundancy_checks
	uint8_t crc = 0xff;
	uint8_t n;

	uint8_t *end = data + len;
	for( ; data < end; data++) {
		crc ^= *data;
		for(n = 0; n < 8; n++) {
			if(crc & 0x01) {
				crc = (crc >> 1) ^ mask;
			}
			else {
				crc >>= 1;
			}
		}
	}
	return ~crc;
}



