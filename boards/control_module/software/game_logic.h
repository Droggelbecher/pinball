
#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include "switches.h"
#include "solenoids.h"
//#include "pcf_font.h"
#include "state_buffer.h"
//#include "display.h"
//#include "canvas.h"
#include "spi.h"

class GameLogic {

	public:
		GameLogic();
		void next_frame();

	private:
		//PcfFont font_normal { "gohufont-11.pcf" };

		Spi spi;

		Switches switches { spi };
		StateBuffer<Switches> switches_delta { switches };

		Solenoids solenoids { spi };
		//StateBuffer<Solenoids> solenoids_delta { solenoids };

		//Display display;

		//ScrollingCanvas marquee { display, { -20, 0 } };
};


#endif // GAMELOGIC_H

