
#ifndef CHECKSUM_H
#define CHECKSUM_H

#include <stdint.h>

/**
 * Compute checksum over a stream of bytes.
 * This implementation uses CRC8 CCIT.
 */
uint8_t checksum(uint8_t* data, uint8_t len);

#endif // CHECKSUM_H

