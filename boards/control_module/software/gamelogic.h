
#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include "switches.h"
#include "solenoids.h"

class Gamelogic {

	public:
		Gamelogic();
		void next_frame();

	private:
		Pcf font_normal { "gohufont-11.pcf" };

		Switches switches;
		StateBuffer<Switches> switches_delta { switches };

		Solenoids solenoids;
		StateBuffer<Solenoids> solenoids_delta { solenoids };

		Display display;

		ScrollingCanvas marquee(display, { -20, 0 });
};


#endif // GAMELOGIC_H

