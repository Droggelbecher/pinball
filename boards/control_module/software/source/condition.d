
import std.datetime: Duration;
import std.datetime.systime: Clock, SysTime;

interface Condition {
	bool opCall();
}

class DelegateCondition : Condition {

	alias dg this;

	this(bool delegate() dg) {
		this.dg = dg;
	}

	override bool opCall() { return this.dg(); }

	private {
		bool delegate() dg;
	}
}

Condition make_condition(bool delegate() dg) {
	return new DelegateCondition(dg);
}

class TrueCondition : Condition {
	override bool opCall() { return true; }
}

Condition make_condition() {
	return new TrueCondition();
}

class WaitCondition : Condition {
	this(Duration duration) {
		this.active_time = Clock.currTime + duration;
	}

	override bool opCall() {
		return active_time <= Clock.currTime;
	}

	private {
		SysTime active_time;
	}
}

Condition make_condition(Duration duration) {
	return new WaitCondition(duration);
}


