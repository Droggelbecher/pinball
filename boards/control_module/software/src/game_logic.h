
#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include "audio/audio.h"
#include "canvas/scrolling_canvas.h"
#include "canvas/broadcast_canvas.h"
#include "state_buffer.h"
#include "framer.h"
#include "pcf_font.h"
#include "keep_value_delay.h"
#include "config.h"

#include <memory>

#define GAME_LOGIC_TEMPL(TTT) template<typename InterfaceT> TTT GameLogic<InterfaceT>

template<typename InterfaceT>
class GameLogic {
    using Interface = InterfaceT;
  public:
    GameLogic(Interface& interface);
    void next_frame(double dt);

  private:
    Interface& interface;
    Audio audio { Audio::instance() };
    PcfFont font_normal { "resources/gohufont-11.pcf" };

    Framer framer { DISPLAY_TARGET_FPS };
    StateBuffer<typename Interface::Switches> switches_delta { interface.switches() };

    // Delay (debounce) ball return by a bit as to make sure ball has rolled all the way down first.
    KeepValueDelay ball_return {
      [this]() -> bool { return interface.switches().get(Interface::Switches::Index::BALL_OUT); },
      true, 1000
    };

    ScrollingCanvas marquee { interface.canvas(), { 0.0, 10.0 } };

    Audio::audio_source_t sound_r2d2_again;
    Audio::audio_source_t sound_death_star_explode;

};

#include "game_logic.cc"

#endif // GAMELOGIC_H

/* vim: set ts=2 sw=2 tw=78 noexpandtab :*/
