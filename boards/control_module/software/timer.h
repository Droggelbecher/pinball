
#ifndef TIMER_H
#define TIMER_H

struct timer_state {
	unsigned long target_frame_;
};

void timer_compute(struct timer_state *);
void timer_set(struct timer_state *, double target_time);
uint8_t timer_fired(struct timer_state *);

#endif // TIMER_H

