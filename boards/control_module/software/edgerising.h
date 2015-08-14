
#ifndef EDGERISING_H
#define EDGERISING_H

struct edgerising_state {
	uint8_t value_;
};

uint8_t edgerising_compute(struct edgerising_state *, uint8_t);
void edgerising_init(struct edgerising_state *, uint8_t);

#endif // EDGERISING_H


