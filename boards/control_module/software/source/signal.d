
import std.datetime: Duration, msecs;
import std.experimental.logger;
import std.variant;

class Signal {

	//this(bool delegate() pure @safe f, bool active_side = true) {
		////infof("Signal(delegate) %d %d", f(), active_side);
		//this.f = f;
		//this.active_side = active_side;
	//}

	this(bool delegate() f, bool active_side = true) {
		//infof("Signal(delegate) %d %d", f(), active_side);
		this.f = f;
		this.active_side = active_side;
	}

	this(Signal f, bool active_side = true) {
		//infof("Signal(Signal) %d %d", f(), active_side);
		this.f = f;
		this.active_side = active_side;
	}
	
	// Note: This does not work as expected when calling eg with iface.switches[Sw.BALL_OUT] as lazy bool argument.
	// Instead use delegate form: () => iface.switches[...]
	//this(lazy bool b, bool active_side = true) {
		//auto dg = &b;
		//this.f = &b;
		////infof("Signal(lazy) -> %s / %s / %s / %s", typeid(this.f), typeid(b), typeid(dg), typeid(() => b()));
		//this.active_side = active_side;
	//}

	void frame_start(Duration dt) {
		bool v;
		if(f.convertsTo!Signal) {
			Signal s = f.get!Signal;
			s.frame_start(dt);
			v = s();
		}
		else if(f.convertsTo!(bool delegate())){
			v = f.get!(bool delegate())()();
		}
		else if(f.convertsTo!(bool delegate() pure @safe)){
			v = f.get!(bool delegate() pure @safe)()();
		}

		this.output = process_value(v == active_side, dt);
	}

	bool process_value(bool v, Duration dt) {
		return v;
	}

	bool opCast(T: bool)() {
		return output;
	}

	bool opCall() {
		return output;
	}

	private:
		Algebraic!(Signal, bool delegate() pure @safe, bool delegate()) f;
		bool output;

	protected:
		bool active_side;
}

class KeepValueDelay: Signal {

	/**
		                _______
		   __/\___/\___/       \______
		                  _____
		-> ______________/     \______
		
		fx: Delegate that yields the value to debounce
		active_side: Value that is considered "active"
		delay: time that fx() must yield the "active" value for the output (opCast) to yield active
	*/
	this(T)(T f, bool active_side, Duration delay) {
		super(f, active_side);
		this.delay = delay;
		this.active_time = 0.msecs;
	}

	override
	bool process_value(bool v, Duration dt) {
		if(v) {
			// optimistic in the sense that it assumes it was active the whole frame
			active_time += dt;
		}
		else {
			active_time = 0.msecs;
		}
		return active_time >= delay ? active_side : !active_side;
	}

	private:
		Duration delay;
		Duration active_time;
}

class Rising: Signal {
	/**
		Deliver the active_side value only in the first frame it appears.
		Reset if value is !active_side.
		
		                _______
		   __/\___/\___/       \______
		
		-> __|____|____|______________
	*/

	this(T)(T f, bool active_side = true) {
		super(f, active_side);
		this.state = true;
	}

	override
	bool process_value(bool v, Duration _) {
		bool r = v && !state;
		state = v;
		return r;
	}

	private:
		bool state;
}

