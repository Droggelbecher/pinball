
#include "utils.h"

uint8_t reverse_byte(uint8_t b) {
	// source: https://graphics.stanford.edu/~seander/bithacks.html#ReverseByteWith64BitsDiv
	return (uint8_t)((b * 0x0202020202ULL & 0x010884422010ULL) % 1023);
}

void swap_bytes(uint8_t *p, size_t sz) {
	int i = 0;
	for( ; i < sz; i+=2) {
		p[i] ^= p[i+1]; // p'[i] = p[i] ^ p[i+1]
		p[i+1] ^= p[i];  // p1[i+1= p[i+1] ^ p[i] ^ p[i+1] = p[i]
		p[i] ^= p[i+1];
	}
}

uint8_t is_big_endian(void) {
	union {
		uint32_t i;
		char c[4];
	} bint = {0x01020304};

	return bint.c[0] == 1; 
}


int sgn(int16_t x) {
	return (x < 0) ? -1 : (x > 0);
}

