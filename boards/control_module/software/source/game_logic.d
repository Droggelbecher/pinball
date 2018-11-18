
//import std.datetime.systime : Clock;

import task : Task;
//import mock_spi;

class GameLogic(Solenoids, Switches, Display) : Task {

	import std.stdio : writeln;
	import std.datetime : seconds, msecs;

	Solenoids solenoids;
	Switches switches;
	Display display;

	alias Sol = Solenoids.Index;
	alias Sw = Switches.Index;

	this(Solenoids solenoids, Switches switches, Display display) {
		this.solenoids = solenoids;
		this.switches = switches;
		this.display = display;
	}

	override void frame_start(double dt) {
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

		/+
		solenoids[Sol.BALL_RETURN] 

		if(switches_delta.rising(Sw.HOLE0)) {
			//iface.play_sound(sound_death_star_explode);
		}

		solenoids[Sol.BALL_RETURN] = ball_return();
		+/

	}

	override void run() {

		foreach(int row; 0 .. display.size.row) {
			foreach(int column; 0 .. display.size.column) {
				display[row, column] = ((row + column) % 2) ? 5 : 0;
			}
		}

		yield(1.seconds);

		foreach(int row; 0 .. display.size.row) {
			foreach(int column; 0 .. display.size.column) {
				display[row, column] = ((row + column) % 2) ? 0 : 5;
			}
		}

		/*
		writeln("First line ", Clock.currTime());
		yield(2.seconds);
		writeln("Initializing state ", Clock.currTime());
		yield(3.seconds);
		writeln("3s later ", Clock.currTime());
		*/
	}
}
