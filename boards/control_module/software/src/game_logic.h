
#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include "audio.h"
#include "switches.h"
#include "canvas/scrolling_canvas.h"
#include "canvas/broadcast_canvas.h"
#include "lamps.h"
#include "solenoids.h"
#include "spi.h"
#include "state_buffer.h"
#include "framer.h"
#include "pcf_font.h"
#include "keep_value_delay.h"
#include "display/curses_display.h"
#include "display/spi_display.h"

#include <memory>

class GameLogic {

	public:
		GameLogic();
		void next_frame();

	private:
		Audio audio { Audio::instance() };
		PcfFont font_normal { "resources/gohufont-11.pcf" };

		Spi spi;
		Framer framer { 30 };

		Switches switches { spi };
		StateBuffer<Switches> switches_delta { switches };

    KeepValueDelay ball_return {
      [this]() -> bool { return !switches.get(Switches::BALL_OUT); },
      framer, true, 1000000
    };


		Solenoids solenoids { spi };

		//CursesDisplay display { Coordinate<>(16, 3 * 8) };
		BroadcastCanvas display {
			std::unique_ptr<CursesDisplay> { new CursesDisplay { Coordinate<>(16, 3 * 8) } }
			//std::unique_ptr<SpiDisplay> { new SpiDisplay { spi, 3, Coordinate<>(16, 8) } }
		};
		ScrollingCanvas marquee { display, { 0.0, 0.6 } };


		Audio::audio_source_t sound_r2d2_again;
		Audio::audio_source_t sound_death_star_explode;

};


#endif // GAMELOGIC_H

/* vim: set ts=2 sw=2 tw=78 noexpandtab :*/
