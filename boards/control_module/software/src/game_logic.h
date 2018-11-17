
#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include "audio/audio.h"
#include "canvas/scrolling.h"
#include "canvas/broadcast_canvas.h"
#include "state_buffer.h"
#include "framer.h"
#include "font.h"
#include "keep_value_delay.h"
#include "config.h"
#include "gohu.h"
#include "5x8.h"

#include <memory>

#define GAME_LOGIC_TEMPL(TTT) template<typename InterfaceT> TTT GameLogic<InterfaceT>

namespace pinball {

template<typename InterfaceT>
class GameLogic {
		using Interface = InterfaceT;
	public:
		GameLogic(Interface& interface);
		void next_frame(double dt);

	private:
		Interface& interface;
		Audio audio { Audio::instance() };
		Font<font_gohu_size> font_normal { font_gohu_data };
		Font<font_5x8_size> font_small { font_5x8_data };

		Framer framer { DISPLAY_TARGET_FPS };
		StateBuffer<typename Interface::Switches> switches_delta { interface.switches() };

		// Delay (debounce) ball return by a bit as to make sure ball has rolled all the way down first.
		KeepValueDelay ball_return {
			[this]() -> bool { return interface.switches().get(Interface::Switches::Index::BALL_OUT); },
			true, 1000
		};

		canvas::Scrolling<typename Interface::Canvas> marquee {
			interface.canvas(),
			{ 30, DISPLAY_MODULE_COLUMNS * DISPLAY_MODULE_COUNT },
			{ -10.0, 0.0 }
		};

		Audio::audio_source_t sound_r2d2_again;
		Audio::audio_source_t sound_death_star_explode;
};

} // ns pinball

#include "game_logic.cc"

#endif // GAMELOGIC_H

/* vim: set ts=2 sw=2 tw=78 noexpandtab :*/
