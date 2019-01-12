
import task: Task;
import utils: assumeNoGC;

class GameLogic(Solenoids, Switches, Display) : Task {

	import std.stdio:    writeln;
	import std.datetime: Duration, seconds, msecs;
	import font:         Font, StringCanvas;
	import five_eight:   font_5x8_size, font_5x8_data;
	import coordinate;
	import canvas:       blit, clear, set_color;
	import scrolling;
	import keep_value_delay;
	import audio;

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

	KeepValueDelay ball_return;

	bool show_text;
	bool enable_ball_return;

	AudioSource main_theme;
	Playlist playlist;

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
		this.enable_ball_return = false;

		// Debounce ball out switch in the sense
		// that we want it to be "true" for at least 1000 msecs
		// before reacting so ball has really come to halt
		this.ball_return = KeepValueDelay(
			() => switches[Sw.BALL_OUT], true, 1000.msecs
		);

		//this.main_theme = load_sound("./resources/music/original/01_IV_main_theme.mp3");
		this.playlist = new Playlist(
			"./resources/music/original/01_IV_main_theme.mp3"
		);
	}

	@nogc
	override void frame_start(Duration dt) {
		switches.frame_start(dt);
		ball_return.frame_start(dt);

		// Some more or less direct switch -> solenoid mappings
		{
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

			if(enable_ball_return && ball_return) {
				solenoids[Sol.BALL_RETURN] = true;
			}
		}
		solenoids.frame_start(dt);

		// Now care for display-related updates

		marquee.next_frame(dt);

		display.clear;
		if(show_text) {
			scrolling.blit(text, Coord(), text.size, marquee, Coord());
		}

		assumeNoGC(&playlist.frame_start)(dt);
	}

	void blank(Duration t = 100.msecs) {
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
		//blank(4200.msecs); main_theme.play(); blank(800.msecs);
		blank(4200.msecs);
		//playlist.frame_start(0.msecs);
		playlist.play();
		//playlist.frame_start(0.msecs);
		blank(800.msecs);

		text = font_normal("  STAR  \n  WARS  \n\n\n Ep. IV \n\n  A new \n  hope  ", 3);
		show_text = true;

		yield(2000.msecs);
		marquee.speed = Coordinate!double(-5, 0);

		yield(9700.msecs);
		marquee.stop;
		yield(4000.msecs);
		blank(4000.msecs);

		marquee.reset;
		text = font_normal(" READY  \nPLAYER 1", 2);
		yield(1000.msecs);
		blink(500.msecs);
		show_text = true;
		yield(1000.msecs);
		blank();
	}

	override void run() {
		intro();
		enable_ball_return = true;
	}
}

