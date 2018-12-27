
import task : Task;

class GameLogic(Solenoids, Switches, Display) : Task {

	import std.stdio : writeln;
	import std.datetime : seconds, msecs;
	import font: Font, StringCanvas;
	import five_eight: font_5x8_size, font_5x8_data;
	import coordinate;
	import canvas: blit, clear;
	import scrolling: Scrolling, blit;

	Solenoids solenoids;
	Switches switches;
	Display display;

	alias Font!(font_5x8_size) FontNormal;

	FontNormal font_normal;
	Scrolling!Display marquee;

	alias Sol = Solenoids.Index;
	alias Sw = Switches.Index;
	alias C = Coordinate!();

	this(Solenoids solenoids, Switches switches, Display display) {
		this.solenoids = solenoids;
		this.switches = switches;
		this.display = display;

		this.font_normal = new FontNormal(font_5x8_data);
		this.marquee = new Scrolling!Display(display, display.size, Coordinate!double(5.0, 0.0));
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

		marquee.next_frame(dt);

		display.clear;
		auto text = font_normal("Hello");
		//blit(text, C(), text.size, display, C());

		blit!(StringCanvas!FontNormal, Display)(
				text, Coord(), text.size,
				marquee, Coord()
		);

	}

	override void run() {
		foreach(int row; 0 .. display.size.row) {
			foreach(int column; 0 .. display.size.column) {
				display[row, column] = ((row + column) % 2) ? 5 : 0;
			}
		}
		yield(100.msecs);

		foreach(int row; 0 .. display.size.row) {
			foreach(int column; 0 .. display.size.column) {
				display[row, column] = ((row + column) % 2) ? 0 : 5;
			}
		}
		yield(100.msecs);

		display.clear;
		yield(100.msecs);

		//blit(font_normal['b'], C(), font_normal.size, display, C());
		/*
		writeln("First line ", Clock.currTime());
		yield(2.seconds);
		writeln("Initializing state ", Clock.currTime());
		yield(3.seconds);
		writeln("3s later ", Clock.currTime());
		*/
	}
}

