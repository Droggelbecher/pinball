
import std.datetime: Duration, seconds, msecs;

import task: Task;
import signal;
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
		Rising[] dtb_scored;
		Rising   dtb_all_scored;

		Rising   hole0_hit;
	}

	private {
		KeepValueDelay ball_return;
	}

	this() {
		// Debounce ball out switch in the sense
		// that we want it to be "true" for at least 1000 msecs
		// before reacting so ball has really come to halt
		this.ball_return = KeepValueDelay(() => iface.switches[Sw.BALL_OUT], true, 1000.msecs);
		this.dtb_scored = [
			Rising(() => iface.switches[Sw.DTB0_0]),
			Rising(() => iface.switches[Sw.DTB0_1]),
			Rising(() => iface.switches[Sw.DTB0_2]),
			Rising(() => iface.switches[Sw.DTB0_3]),
			Rising(() => iface.switches[Sw.DTB0_4]),
		];
		this.dtb_all_scored = Rising(() => (
			   iface.switches[Sw.DTB0_0]
			&& iface.switches[Sw.DTB0_1]
			&& iface.switches[Sw.DTB0_2]
			&& iface.switches[Sw.DTB0_3]
			&& iface.switches[Sw.DTB0_4]
		));

		this.hole0_hit = Rising(() => iface.switches[Sw.HOLE0], false);
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

	bool enabled_condition() {
		return !resetting;
	}

	override
	void frame_start(Duration dt) {

		if(!enabled) {
			return;
		}

		ball_return.frame_start(dt);
		foreach(ref dtb; dtb_scored) {
			dtb.frame_start(dt);
		}
		dtb_all_scored.frame_start(dt);
		hole0_hit.frame_start(dt);

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

			solenoids[Sol.BALL_RETURN] = cast(bool)ball_return;
		}
	} // frame_start()
}
