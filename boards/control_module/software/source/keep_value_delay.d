
import std.datetime: Duration, msecs;

struct KeepValueDelay {

	this(bool delegate() @nogc fx, bool active_side, Duration delay) {
		this.f = fx;
		this.active_side = active_side;
		this.delay = delay;
		this.active_time = 0.msecs;
	}

	@nogc
	void frame_start(Duration dt) {
		bool v = f();
		if(v == active_side) {
			// optimistic in the sense that it assumes it was active the whole frame
			active_time += dt;
		}
		else {
			active_time = 0.msecs;
		}
	}

	bool opCast(T: bool)() {
		return active_time >= delay ? active_side : !active_side;
	}

	bool delegate() @nogc f;
	bool active_side;
	Duration delay;
	Duration active_time;
}

