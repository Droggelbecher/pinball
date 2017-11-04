
#include "framer.h"

#include <time.h>
#include <unistd.h>
#include <stdio.h>

#include <iostream>

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#include <sys/time.h>
#endif

Framer::Framer(int32_t framerate)
	: frame_length { 1000000 / framerate },
	frame_start { get_time_us() } {

}

void Framer::next_frame() {
	frame_start = get_time_us();
}

int32_t Framer::get_last_frame_duration_us() {
	return last_frame_duration;
}

void Framer::wait_frame_end() {
	static int32_t error = 0;
	int32_t diff = (last_frame_duration - frame_length);
	error += diff;

	int32_t length_so_far = get_time_us() - frame_start;
	int32_t wait_time = frame_length - length_so_far - 0.5 * diff - 0.1 * error;

	if(wait_time > 0) {
		wait_us(wait_time);
	}

	last_frame_duration = get_time_us() - frame_start;
}

int32_t Framer::get_time_us() {
	int32_t us; // Microseconds

#ifdef __MACH__
	timeval time;
	gettimeofday(&time, NULL);
	us = time.tv_sec * 1000000 + time.tv_usec;
#else
	time_t s;  // Seconds
	struct timespec spec;
	clock_gettime(CLOCK_MONOTONIC, &spec);
	us = spec.tv_sec * 1000000 + (spec.tv_nsec / 1.0e3);
#endif
	return us;
}

void Framer::wait_us(int32_t us) {
	usleep(us);
}


