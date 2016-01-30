
#ifndef CHECKSUM_H
#define CHECKSUM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * Compute checksum over a stream of bytes.
 * This implementation uses CRC8 CCIT.
 */
uint8_t checksum(uint8_t* data, uint8_t len);

#ifdef __cplusplus
}
#endif

#endif // CHECKSUM_H

