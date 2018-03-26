
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

Framer::Framer(double framerate)
: frame_length { static_cast<int64_t>(1000000 / framerate) },
	frame_start { get_time_us() } {

}

void Framer::next_frame() {
	wait_frame_end();
	frame_start = get_time_us();
}

int64_t Framer::get_last_frame_duration_us() {
	return last_frame_duration;
}

void Framer::wait_frame_end() {
	// error = integrated part of error in control loop
	static int64_t error = 0;

	// By how much was the last frame too long? (proportional part)
	int64_t diff = (last_frame_duration - frame_length);

	// Sanity check: limit diff to frame length,
	// some weirdly large values might pop up eg on start when last_frame_duration is 0
	if(diff > frame_length) { diff = frame_length; }
	if(diff < -frame_length) { diff = -frame_length; }

	error += diff;

	int64_t length_so_far = get_time_us() - frame_start;

	// Ideally we would neet to wait (frame_length - length_so_far) and be done with it.
	// However there is some overhead which is neglected by this
	// (loop, this calculation, sleep inacuracy, etc..).
	// Model this as a PI control problem
	// 
	// 0.5 * P + 0.1 * I
	int64_t wait_time = frame_length - length_so_far
		- 0.3 * diff
		- 0.7 * error;

	//std::cerr << "l:" << frame_length << " ll:" << length_so_far
		//<< " d:" << diff
		//<< " e:" << error
		//<< " w:" << wait_time << std::endl;

	if(wait_time > 0) {
		wait_us(wait_time);
	}

	last_frame_duration = get_time_us() - frame_start;
}

int64_t Framer::get_time_us() {
	int64_t us; // Microseconds

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

void Framer::wait_us(int64_t us) {
	usleep(us);
}


