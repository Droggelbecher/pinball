
import std.datetime;
import std.stdio;
import core.thread: Fiber;

import condition;

class Task {

	this() {
		fiber = new Fiber(&run);
	}

	this(Args...)(Args args) {
		fiber = new Fiber(&run);
		condition = make_condition(args);
	}

	void frame_start(Duration dt) { }
	void run() { }

	void check_run() {
		if(fiber.state != Fiber.State.TERM && condition()) {
			fiber.call();
		}
	}

	void yield(Args...)(Args args) {
		condition = make_condition(args);
		fiber.yield();
	}

	void quit() {
		want_quit = true;
	}

	bool quitting() {
		return want_quit;
	}

	private {
		Condition condition = new TrueCondition;
		Fiber fiber;
		bool want_quit = false;
	}
}

