
import std.datetime.systime : SysTime, Clock;
import std.concurrency : Generator, yield;
import std.datetime;
import std.stdio;
import core.thread : Fiber;

import condition;

class Task {

	this() {
		fiber = new Fiber(&run);
	}

	this(Args...)(Args args) {
		fiber = new Fiber(&run);
		condition = make_condition(args);
	}

	void frame_start(double dt) { }
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

	private {
		Condition condition = new TrueCondition;
		Fiber fiber;
	}
}

