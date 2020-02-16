
import std.datetime: Duration, seconds, msecs;

import task: Task;
import switchable;
import signal: Rising;

class SwitchesAsInput(alias iface): Task {

	alias Interface = typeof(iface);
	alias Sw = Interface.Switches.Index;

	mixin Switchable;

	enum Command {
		NONE, NEXT, SELECT
	}

	public {
		Rising left, right;
		Command command;
	}

	this() {
		this.left = new Rising(() => iface.switches[Sw.FLIPPER_LEFT], true);
		this.right = new Rising(() => iface.switches[Sw.FLIPPER_RIGHT], true);
		this.command = Command.NONE;
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

	Command query() {
		Command c = command;
		command = Command.NONE;
		return c;
	}
}

