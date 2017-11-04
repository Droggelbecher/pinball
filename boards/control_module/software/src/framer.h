
#ifndef FRAMER_H
#define FRAMER_H

#include <stdint.h>

class Framer {
	public:

		Framer(int framerate);

		void next_frame();
		void wait_frame_end();
		int32_t get_last_frame_duration_us();

	private:

		static int get_time_us();
		static void wait_us(int32_t);

		int32_t frame_length;
		int32_t frame_start;
		int32_t last_frame_duration;

		double avg_real_length;
		static constexpr double real_length_alpha = .01;

};


#endif // FRAMER_H

