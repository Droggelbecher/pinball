
#ifndef FRAMER_H
#define FRAMER_H

#include <stdint.h>

class Framer {
	public:

		Framer(double framerate);

		void next_frame();
		void wait_frame_end();
		int64_t get_last_frame_duration_us();

	private:
		static int64_t get_time_us();
		static void wait_us(int64_t);

		int64_t frame_length;
		int64_t frame_start;
		int64_t last_frame_duration;
};


#endif // FRAMER_H

