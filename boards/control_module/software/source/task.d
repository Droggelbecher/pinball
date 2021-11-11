
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

	/**
	  If true, keep this task scheduled for its frame_start effect
	  when run() has returned.
	  */
	bool daemon = true;

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

	/**
	  Request to exit the whole scheduling process.
	  */
	void quit() {
		want_quit = true;
	}

	/**
	  Schedule another task.
	  */
	void schedule(Task task, int relative_priority = 0) {
		to_schedule ~= ScheduleRequest(task, relative_priority);
	}

	/**
	  Schedule another task.
	  */
	void schedule(void delegate() dg, int relative_priority = 0) {
		this.schedule(new DelegateTask(dg), relative_priority);
	}

	//
	// For use from scheduler side...
	//

	/**
	  True if this task requests to end the whole scheduling process.
	  */
	bool quitting() {
		return want_quit;
	}

	/**
	  Check if another chunk of run() should be ran and run it if so.
	  */
	void check_run() {
		if(fiber.state != Fiber.State.TERM && condition()) {
			fiber.call();
		}
	}

	/**
	  True iff run() is still active (run() did not return yet but may be yielding currently).
	  This task may still be doing things in `frame_start()` if this returns false.
	  */
	bool running() {
		return fiber.state != Fiber.State.TERM;
	}

	/**
	  Start run() from anew. Should only be called if `running()` returned false.
	  */
	void rerun() {
		fiber.reset;
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


