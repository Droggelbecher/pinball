
import std.datetime;
import std.stdio;
import core.thread: Fiber;

import condition;

class Task {

	int priority = 0;

	this() {
		fiber = new Fiber(&run);
	}

	//
	// For override within task
	//

	void frame_start(Duration dt) { }

	void run() { }

	//
	// For use from within task
	//

	void yield(Args...)(Args args) {
		condition = make_condition(args);
		fiber.yield();
	}

	void quit() {
		want_quit = true;
	}

	void schedule(Task t) {
		to_schedule ~= t;
	}

	//
	// For use from scheduler side...
	//

	bool quitting() {
		return want_quit;
	}

	void check_run() {
		if(fiber.state != Fiber.State.TERM && condition()) {
			fiber.call();
		}
	}

	Task[] pop_schedule_requests() {
		Task[] r = this.to_schedule;
		this.to_schedule = [];
		return r;
	}

	private {
		Condition condition = new TrueCondition;
		Fiber fiber;
		bool want_quit = false;
		Task[] to_schedule;
	}
}


