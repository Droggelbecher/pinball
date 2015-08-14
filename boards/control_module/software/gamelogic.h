
#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include "switches.h"
#include "solenoids.h"

void gamelogic_compute(void);


class Gamelogic {

	public:
		Gamelogic();
		void compute();

	private:
		PcfFont font_normal { "gohufont-11.pcf" };

		Switches switches;
		StateBuffer<Switches> switches_delta { switches };

		Solenoids solenoids;
		StateBuffer<Solenoids> solenoids_delta { solenoids };

		Display display;

		ScrollingCanvas marquee(display, { -20, 0 });
};


#endif // GAMELOGIC_H

