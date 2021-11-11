
import std.datetime: Duration, seconds, msecs;

import task: Task;
import switchable;
import signal: Rising;

class SwitchesAsInput(Switches_): Task {

	//alias Interface = typeof(iface);
	alias Switches = Switches_;
	alias Sw = Switches.Index;

	mixin Switchable switchable;

	enum Command {
		NONE, NEXT, SELECT
	}

	public {
		Rising left, right;
		Command command;
	}

	this(Switches switches) {
		this.left = new Rising(() => switches[Sw.FLIPPER_LEFT], true);
		this.right = new Rising(() => switches[Sw.FLIPPER_RIGHT], true);
		this.command = Command.NONE;
		this.switches = switches;
	}

	override void frame_start(Duration dt) {
		left.frame_start(dt);
		right.frame_start(dt);
		
		if(!enabled) {
			return;
		}

		if(left()) {
			command = Command.NEXT;
		}
		else if(right()) {
			command = Command.SELECT;
		}
	}

	bool has_command() {
		return command != Command.NONE;
	}

	Command pop_command() {
		Command c = command;
		command = Command.NONE;
		return c;
	}

	void on(bool enabled=true) {
		if(enabled) {
			// When turned on, reset switches states, otherwise
			// we might accidentially fire an event based on
			// switch presses that happened before
			this.left.reset;
			this.right.reset;
		}
		switchable.on(enabled);
	}

	private {
		Switches switches;
	}
}

