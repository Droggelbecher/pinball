
import std.datetime: Duration, seconds, msecs;

import task: Task;
import signal;

class PlayingField(alias iface) : Task {

	alias Interface = typeof(iface);
	alias Sol = Interface.Solenoids.Index;
	alias Sw = Interface.Switches.Index;

	public {
		bool     enable_ball_return;
		Rising[] dtb_scored;
		Rising   dtb_all_scored;
	}

	private {
		KeepValueDelay ball_return;
	}

	this() {
		this.enable_ball_return = false;

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
	}

	override
	void frame_start(Duration dt) {
		ball_return.frame_start(dt);
		foreach(ref dtb; dtb_scored) {
			dtb.frame_start(dt);
		}
		this.dtb_all_scored.frame_start(dt);

		with(iface) {
			solenoids[Sol.FLIPPER_LEFT]  = switches[Sw.FLIPPER_LEFT];
			solenoids[Sol.FLIPPER_RIGHT] = switches[Sw.FLIPPER_RIGHT];
			solenoids[Sol.SLINGSHOT0]    = switches[Sw.SLINGSHOT0];
			solenoids[Sol.SLINGSHOT1]    = switches[Sw.SLINGSHOT1];
			solenoids[Sol.BUMPER0]       = switches[Sw.BUMPER0];
			solenoids[Sol.BUMPER1]       = switches[Sw.BUMPER1];
			solenoids[Sol.BUMPER2]       = switches[Sw.BUMPER2];

			solenoids[Sol.DTB0] = 
				   switches[Sw.DTB0_0]
				&& switches[Sw.DTB0_1]
				&& switches[Sw.DTB0_2]
				&& switches[Sw.DTB0_3]
				&& switches[Sw.DTB0_4];

			solenoids[Sol.BALL_RETURN] = enable_ball_return && ball_return;
		}
	} // frame_start()
}