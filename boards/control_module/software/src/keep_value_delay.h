
#ifndef KEEP_VALUE_DELAY_H
#define KEEP_VALUE_DELAY_H

#include <functional>
#include <cassert>

/**
 * Wrap a boolean value-getter function $f such that
 * $this->get() yields only $active (true/false) if $f() returned
 * $active for at least $delay_ms milliseconds (according to observations in the previous frames)
 */
class KeepValueDelay {
  
  public:
    KeepValueDelay(std::function<bool()> f, bool active_side, int32_t delay_ms)
      : function(f), active_side(active_side), delay_ms(delay_ms), active_time_ms(0) {
    }

    void next_frame(double dt) {
      bool v = function();
      if(v == active_side) {
        // optimistic in the sense that it assumes it was active the whole frame
        active_time_ms += static_cast<int32_t>(dt * 1000.0);
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
    bool active_side;
    int32_t delay_ms;
    int32_t active_time_ms;
};

#endif // KEEP_VALUE_DELAY_H

/* vim: set ts=2 sw=2 tw=78 noexpandtab :*/
