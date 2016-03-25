
#ifndef EDGERISING_H
#define EDGERISING_H

#include <stdint.h>

struct edgerising_state {
	uint8_t value_;
};

uint8_t edgerising_compute(struct edgerising_state *, uint8_t);
void edgerising_init(struct edgerising_state *, uint8_t);

uint8_t limit(uint8_t v, uint8_t min, uint8_t max);

#endif // EDGERISING_H


