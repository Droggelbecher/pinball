
import std.datetime: Duration, seconds, msecs;

import task: Task;
import signal: KeepValueDelay, Rising, Signal;
import switchable;

class PlayingField(alias iface) : Task {

	alias Interface = typeof(iface);
	alias Sol = Interface.Solenoids.Index;
	alias Sw = Interface.Switches.Index;
	alias Lamp = Interface.LEDStripe.Lamp;

	mixin Switchable;

	public {
		// Overall state of the playing field
		bool     resetting;

		// Playing field items abstractions
		Signal[] dtb_scored;
		Signal   dtb_all_scored;

		Signal   hole0_hit;
		Signal   spinner_scored;
		Signal   bumper_scored;

		//KeepValueDelay ball_out;
		Signal   ball_out;
	}

	private {
		bool _enable_ball_return = false;
	}

	this() {
		// Debounce ball out switch in the sense
		// that we want it to be "true" for some time
		// before reacting so ball has really come to halt
		this.ball_out = new Rising(
				new KeepValueDelay(() => iface.switches[Sw.BALL_OUT], true, 500.msecs),
		);
		this.dtb_scored = [
			new Rising(() => iface.switches[Sw.DTB0_0]),
			new Rising(() => iface.switches[Sw.DTB0_1]),
			new Rising(() => iface.switches[Sw.DTB0_2]),
			new Rising(() => iface.switches[Sw.DTB0_3]),
			new Rising(() => iface.switches[Sw.DTB0_4]),
		];
		this.dtb_all_scored = new Rising(() =>
			   iface.switches[Sw.DTB0_0]
			&& iface.switches[Sw.DTB0_1]
			&& iface.switches[Sw.DTB0_2]
			&& iface.switches[Sw.DTB0_3]
			&& iface.switches[Sw.DTB0_4]
		);

		this.bumper_scored = new Rising(() =>
			   iface.switches[Sw.BUMPER0]
			|| iface.switches[Sw.BUMPER1]
			|| iface.switches[Sw.BUMPER2]
		);

		this.hole0_hit = new Rising(() => iface.switches[Sw.HOLE0], false);
		this.spinner_scored = new Rising(() => iface.switches[Sw.SPINNER], true);
	}

	override
	void run() {

		resetting = true;
		while(iface.switches[Sw.DTB0_0]
				|| iface.switches[Sw.DTB0_1]
				|| iface.switches[Sw.DTB0_2]
				|| iface.switches[Sw.DTB0_3]
				|| iface.switches[Sw.DTB0_4]) {
			iface.solenoids[Sol.DTB0] = true;
			yield(10.msecs);
		}
		iface.solenoids[Sol.DTB0] = false;
		resetting = false;

	}

	override
	void frame_start(Duration dt) {

		// Clear this even if playing field is not enabled,
		// otherwise it can happen that this is on for a long time
		iface.solenoids[Sol.BALL_RETURN] = false;

		if(!enabled) {
			return;
		}

		ball_out.frame_start(dt);
		foreach(ref dtb; dtb_scored) {
			dtb.frame_start(dt);
		}
		dtb_all_scored.frame_start(dt);
		bumper_scored.frame_start(dt);
		hole0_hit.frame_start(dt);
		spinner_scored.frame_start(dt);

		with(iface) {
			solenoids[Sol.FLIPPER_LEFT]  = switches[Sw.FLIPPER_LEFT];
			solenoids[Sol.FLIPPER_RIGHT] = switches[Sw.FLIPPER_RIGHT];
			solenoids[Sol.SLINGSHOT0]    = switches[Sw.SLINGSHOT0];
			solenoids[Sol.SLINGSHOT1]    = switches[Sw.SLINGSHOT1];
			solenoids[Sol.BUMPER0]       = switches[Sw.BUMPER0];
			solenoids[Sol.BUMPER1]       = switches[Sw.BUMPER1];
			solenoids[Sol.BUMPER2]       = switches[Sw.BUMPER2];

			led_stripe[Lamp.BMP0] = switches[Sw.BUMPER0];
			led_stripe[Lamp.BMP1] = switches[Sw.BUMPER1];
			led_stripe[Lamp.BMP2] = switches[Sw.BUMPER2];

			solenoids[Sol.DTB0] = 
				   switches[Sw.DTB0_0]
				&& switches[Sw.DTB0_1]
				&& switches[Sw.DTB0_2]
				&& switches[Sw.DTB0_3]
				&& switches[Sw.DTB0_4];

			if(_enable_ball_return) {
				iface.solenoids[Sol.BALL_RETURN] = iface.switches[Sw.BALL_OUT];
			}
		}
	} // frame_start()

	void return_ball() {
		_enable_ball_return = true;
		while(iface.switches[Sw.BALL_OUT]) {
			//infof("return_ball(): ball is out");
			yield(100.msecs);
		}
		_enable_ball_return = false;
	}
}
