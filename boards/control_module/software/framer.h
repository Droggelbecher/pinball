
#ifndef FRAMER_H
#define FRAMER_H

#include <stdint.h>

class Framer {
	public:

		Framer(int framerate);

		void next_frame();
		void wait_frame_end();

	private:

		static int get_time_ms();
		static void wait_ms();

		int32_t frame_length;
		int32_t frame_start;

		double avg_real_length;
		static const double real_length_alpha = .01;

};


#endif // FRAMER_H

