
#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h> // size_t
#include <stdint.h>

//#define is_msb_first() ( (*(char*)&_is_big_endian) == 0 )

// used by is_msb_first to determine the systems byteorder
//static uint32_t _is_big_endian;

void transform_bytes(uint8_t *source_start, uint8_t *source_end, uint8_t *target_start, uint32_t format);
uint8_t reverse_byte(uint8_t b);
void swap_bytes(uint8_t *p, size_t sz);
uint8_t is_big_endian(void);

int sgn(int16_t x);

#endif // UTILS_H

