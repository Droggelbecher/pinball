

uint8_t edgerising_compute(struct edgerising_state *s, uint8_t v) {
	uint8_t r = v && !s->value_;
	s->value_ = v;
	return r;
}

void edgerising_init(struct edgerising_state *s, uint8_t v) {
	s->value_ = v;
}

