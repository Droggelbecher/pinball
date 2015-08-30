
#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include "switches.h"
#include "solenoids.h"
#include "pcf_font.h"
#include "state_buffer.h"
#include "display.h"
#include "canvas.h"

class GameLogic {

	public:
		GameLogic();
		void compute();

	private:
		PcfFont font_normal { "gohufont-11.pcf" };

		Switches switches;
		StateBuffer<Switches> switches_delta { switches };

		Solenoids solenoids;
		StateBuffer<Solenoids> solenoids_delta { solenoids };

		Display display;

		ScrollingCanvas marquee { display, { -20, 0 } };
};


#endif // GAMELOGIC_H

