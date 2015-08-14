
#include "utils.h"

uint8_t edgerising_compute(struct edgerising_state *s, uint8_t v) {
	uint8_t r = v && !s->value_;
	s->value_ = v;
	return r;
}

void edgerising_init(struct edgerising_state *s, uint8_t v) {
	s->value_ = v;
}

uint8_t limit(uint8_t v, uint8_t min, uint8_t max) {
	if(v < min) { return min; }
	if(v > max) { return max; }
	return v;
}

