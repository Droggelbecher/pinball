
#include "framer.h"

#include <time.h>
#include <unistd.h>
#include <stdio.h>

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#include <sys/time.h>
#endif

Framer::Framer(int32_t framerate)
	: frame_length { 1000 / framerate },
	frame_start { get_time_ms() } {

}

void Framer::next_frame() {
	frame_start = get_time_ms();
}

int32_t Framer::get_last_frame_duration_ms() {
	return last_frame_duration;
}

void Framer::wait_frame_end() {
	int32_t length_so_far = get_time_ms() - frame_start;
	avg_real_length = avg_real_length * (1.0 - real_length_alpha) + length_so_far * real_length_alpha;
	if(length_so_far < frame_length) {
		wait_ms(frame_length - length_so_far);
	}
	last_frame_duration = get_time_ms() - frame_start;
}

int32_t Framer::get_time_ms() {
	int32_t ms; // Milliseconds
	time_t s;  // Seconds
	struct timespec spec;

#ifdef __MACH__
	timeval time;
	gettimeofday(&time, NULL);
	ms = time.tv_sec * 1000 + time.tv_usec / 1000;

	//clock_serv_t cclock;
	//mach_timespec_t mts;
	//host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
	//clock_get_time(cclock, &mts);
	//mach_port_deallocate(mach_task_self(), cclock);
	//ms = mts.tv_sec * 1000 + (mts.tv_nsec / 1.0e6);
#else
	clock_gettime(CLOCK_REALTIME, &spec);
	ms = spec.tv_sec * 1000 + (spec.tv_nsec / 1.0e6);
#endif
	return ms;
}

void Framer::wait_ms(int32_t ms) {
	usleep(1000UL * ms);
}


