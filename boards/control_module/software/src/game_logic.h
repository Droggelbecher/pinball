
#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include "audio.h"
#include "canvas/scrolling_canvas.h"
#include "canvas/broadcast_canvas.h"
#include "lamps.h"
#include "state_buffer.h"
#include "framer.h"
#include "pcf_font.h"
#include "keep_value_delay.h"

#include <memory>

#define _GAME_LOGIC_TEMPL(TTT) template<\
  typename TInterface, TInterface& interface\
  > TTT GameLogic<TInterface, interface>

#define GAME_LOGIC_TEMPL(TTT) template<typename InterfaceT> TTT GameLogic<InterfaceT>

template<typename InterfaceT>
class GameLogic {
    using Interface = InterfaceT;
  public:
    GameLogic(Interface& interface);
    void next_frame();

  private:
    Interface& interface;
    Audio audio { Audio::instance() };
    PcfFont font_normal { "resources/gohufont-11.pcf" };

    Framer framer { 30 };
    StateBuffer<typename Interface::Switches> switches_delta { interface.switches() };

    KeepValueDelay ball_return {
      [this]() -> bool { return !interface.switches().get(Interface::SwitchesIndex::BALL_OUT); },
      framer, true, 1000000
    };

    ScrollingCanvas marquee { interface, { 0.0, 0.6 } };

    Audio::audio_source_t sound_r2d2_again;
    Audio::audio_source_t sound_death_star_explode;

};

#include "game_logic.cc"

#endif // GAMELOGIC_H

/* vim: set ts=2 sw=2 tw=78 noexpandtab :*/
