
import std.datetime: Duration, seconds, msecs;

import task: Task;
import signal: KeepValueDelay, Rising;
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
		Rising   spinner_scored;

		//KeepValueDelay ball_out;
		Rising   ball_out;
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

		if(!enabled) {
			return;
		}

		ball_out.frame_start(dt);
		foreach(ref dtb; dtb_scored) {
			dtb.frame_start(dt);
		}
		dtb_all_scored.frame_start(dt);
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

			//solenoids[Sol.BALL_RETURN] = cast(bool)ball_out;
		}
	} // frame_start()
}
