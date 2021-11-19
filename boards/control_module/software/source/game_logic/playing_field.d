
import std.datetime: Duration, seconds, msecs;

import task: Task;
import signal: KeepValueDelay, Rising, Signal;
import switchable;

/**
  Track overall state of the playing field.
  */
class PlayingField(alias iface) : Task {

	alias Interface = typeof(iface);
	alias Sol = Interface.Solenoids.Index;
	alias Sw = Interface.Switches.Index;
	alias Lamp = Interface.LEDStripe.Lamp;

	mixin Switchable;

	public {
		// Overall state of the playing field
		bool     resetting;

		/// Playing field items abstractions

		// True iff any DTB target scored previous frame
		Signal[] dtb_scored;
		// True iff all DTB targets are down
		Signal   dtb_all_scored;

		// True iff "Hole 0" was hit previous frame
		Signal   hole0_hit;
		// True iff spinner triggered previous frame
		Signal   spinner_scored;
		// True iff any slingshot triggered previous frame
		Signal   slingshot_scored;
		// True iff any bumper triggered previous frame
		Signal   bumper_scored;

		// True iff ball went into out previous frame
		Signal   ball_out;
	}

	private {
		// If true, return ball on `ball_out`
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
		this.spinner_scored = new Rising(() => iface.switches[Sw.SPINNER]);
		this.slingshot_scored = new Rising(() => iface.switches[Sw.SLINGSHOT0] || iface.switches[Sw.SLINGSHOT1]);
	}

	void reset() {
		run();
	}

	override
	void run() {
		resetting = true;

		iface.led_stripe.lamp(Lamp.DS_WEAPON, false);
		iface.led_stripe.lamp(Lamp.DS_LIGHT, false);
		iface.led_stripe.lamp(Lamp.TARGET, false);
		iface.led_stripe.lamp(Lamp.VADER, false);
		iface.led_stripe.lamp(Lamp.FALCON, false);

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
		if(resetting) {
			return;
		}

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
		slingshot_scored.frame_start(dt);

		with(iface) {
			// Playing field is on so connect flippers/slingshots etc.. in the obvious ways
			solenoids[Sol.FLIPPER_LEFT]  = switches[Sw.FLIPPER_LEFT];
			solenoids[Sol.FLIPPER_RIGHT] = switches[Sw.FLIPPER_RIGHT];
			solenoids[Sol.SLINGSHOT0]    = switches[Sw.SLINGSHOT0];
			solenoids[Sol.SLINGSHOT1]    = switches[Sw.SLINGSHOT1];
			led_stripe[Lamp.SS0]         = switches[Sw.SLINGSHOT0];
			led_stripe[Lamp.SS1]         = switches[Sw.SLINGSHOT1];
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

	/**
	  Wait for ball to be out and return it.
	*/
	void return_ball() {
		bool prev_ball_return = _enable_ball_return;
		scope(exit) {
			_enable_ball_return = prev_ball_return;
		}

		_enable_ball_return = true;
		while(iface.switches[Sw.BALL_OUT]) {
			yield(100.msecs);
		}
	}
}
