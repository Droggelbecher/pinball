
#include "framer.h"

#include <time.h>
#include <unistd.h>

Framer::Framer(int32_t framerate)
	: frame_length { 1000 / framerate },
	frame_start { get_time_ms() } {

}

void Framer::next_frame() {
	frame_start = get_time_ms();
}

void Framer::wait_frame_end() {
	int32_t length_so_far = get_time_ms() - frame_start;
	avg_real_length = avg_real_length * (1.0 - real_length_alpha) + length_so_far * real_length_alpha;
	if(length_so_far < frame_length) {
		wait_ms(frame_length - length_so_far);
	}
}

int32_t Framer::get_time_ms() {
	int32_t ms; // Milliseconds
	time_t s;  // Seconds
	struct timespec spec;
	clock_gettime(CLOCK_REALTIME, &spec);
	s = spec.tv_sec;
	ms = s * 1000 + round(spec.tv_nsec / 1.0e6); // Convert nanoseconds to milliseconds
	return ms;
}

void Framer::wait_ms(int32_t ms) {
	usleep(1000UL * ms);
}


