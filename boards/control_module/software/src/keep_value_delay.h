
#ifndef KEEP_VALUE_DELAY_H
#define KEEP_VALUE_DELAY_H

#include <functional>

#include "framer.h"

class KeepValueDelay {
  
  public:
    KeepValueDelay(std::function<bool()> f, Framer& framer, bool active_side, int32_t delay_ms)
      : function(f), framer(framer), active_side(active_side), delay_ms(delay_ms), active_time_ms(0) {
    }

    void next_frame() {
      bool v = function();
      if(v == active_side) {
        // optimistic in the sense that it assumes it was active the whole frame
        active_time_ms += framer.get_last_frame_duration_ms();
      }
      else {
        active_time_ms = 0;
      }
    }

    bool get() {
        return (active_time_ms >= delay_ms) ? active_side : !active_side;
    }

  private:
    std::function<bool()> function;
    Framer& framer;
    bool active_side;
    int32_t delay_ms;
    int32_t active_time_ms;
};

#endif // KEEP_VALUE_DELAY_H

/* vim: set ts=2 sw=2 tw=78 noexpandtab :*/
