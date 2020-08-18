
import std.datetime;
import std.stdio;
import core.thread: Fiber;

import condition;

class Task {

private:
	struct ScheduleRequest {
		Task task;
		int  relative_priority;
	}

public:

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

	void schedule(Task task, int relative_priority = 0) {
		to_schedule ~= ScheduleRequest(task, relative_priority);
	}

	void schedule(void delegate() dg, int relative_priority = 0) {
		this.schedule(new DelegateTask(dg), relative_priority);
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

	ScheduleRequest[] pop_schedule_requests() {
		ScheduleRequest[] r = this.to_schedule;
		this.to_schedule = [];
		return r;
	}

	private {
		Condition condition = new TrueCondition;
		Fiber fiber;
		bool want_quit = false;
		ScheduleRequest[] to_schedule;
	}
}

class DelegateTask: Task {
	this(void delegate() dg) {
		this.dg = dg;
	}

	override void run() {
		this.dg();
	}

	private {
		void delegate() dg;
	}
}


