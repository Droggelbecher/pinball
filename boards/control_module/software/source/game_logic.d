
import task : Task;

class GameLogic(Solenoids, Switches, Display) : Task {

	import std.stdio:    writeln;
	import std.datetime: Duration, seconds, msecs;
	import font:         Font, StringCanvas;
	import five_eight:   font_5x8_size, font_5x8_data;
	import coordinate;
	import canvas:       blit, clear, set_color;
	import scrolling;

	alias Font!(font_5x8_size) FontNormal;
	alias Sol = Solenoids.Index;
	alias Sw = Switches.Index;
	alias C = Coordinate!();

	Solenoids solenoids;
	Switches switches;
	Display display;

	FontNormal font_normal;
	StringCanvas!FontNormal text;
	Scrolling!Display marquee;
	bool show_text;

	this(Solenoids solenoids, Switches switches, Display display) {
		this.solenoids = solenoids;
		this.switches = switches;
		this.display = display;

		this.font_normal = new FontNormal(font_5x8_data);
		this.marquee = new Scrolling!Display(
				display,
				Coord(80, display.size.column),
		);
		this.show_text = false;
		//this.marquee = new Scrolling!Display(display, display.size, Coordinate!double(0.0, 0.0));
		//marquee.offset = Coordinate!double(2.0, 0.0);
	}

	@nogc
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
		if(show_text) {
			scrolling.blit(text, Coord(), text.size, marquee, Coord());
		}
	}

	void blank(Duration t) {
		show_text = false;
		yield(t);
	}

	void blink(Duration duration = 1000.msecs, Duration interval = 100.msecs) {
		auto t = 0.msecs;
		while(t < duration) {
			show_text = !show_text;
			yield(interval);
			t += interval;
		}
	}

	void intro() {
		yield(1000.msecs);
		text = font_normal("  STAR  \n  WARS  \n\n\n Ep. IV ");
		show_text = true;

		yield(2000.msecs);
		marquee.speed = Coordinate!double(-5, 0);

		yield(5500.msecs);
		marquee.stop;
		yield(2000.msecs);
		blank(2000.msecs);

		marquee.reset;
		text = font_normal(" READY  \nPLAYER 1");
		blink();
		show_text = true;
	}

	override void run() {
		intro();

	}
}

