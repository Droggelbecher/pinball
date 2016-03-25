
#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include "audio.h"
#include "switches.h"
#include "canvas.h"
#include "display_canvas.h"
#include "lamps.h"
#include "solenoids.h"
#include "spi.h"
#include "state_buffer.h"
#include "framer.h"

class GameLogic {

	public:
		GameLogic();
		void next_frame();

	private:
		Audio audio { Audio::instance() };
		//PcfFont font_normal { "gohufont-11.pcf" };

		Spi spi;

		Switches switches { spi };
		StateBuffer<Switches> switches_delta { switches };
		Solenoids solenoids { spi };

		DisplayCanvas display { Coordinate<>(16, 3 * 8) };
		ScrollingCanvas marquee { display, { -20, 0 } };

		Framer framer { 30 };

};


#endif // GAMELOGIC_H

/* vim: set ts=2 sw=2 tw=78 noexpandtab :*/