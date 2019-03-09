
import task: Task;
import std.datetime: Duration, seconds, msecs;

class GameLogic(Interface_) : Task {

	import std.stdio:    writeln;
	//import std.format;
	import std.conv;
	import core.stdc.stdio;

	import font:         Font, StringCanvas;
	import five_eight:   font_5x8_size, font_5x8_data;
	import coordinate;
	import canvas:       blit, blit_center, clear, set_color;
	import scrolling;
	import signal;
	//import audio;
	//import audio = mock_audio;
	import audio_mad_al;
	import utils: assumeNoGC;
	import logger: logf;

	alias blit = canvas.blit;
	alias blit = scrolling.blit;

	alias Interface = Interface_;

	alias Font!(font_5x8_size) FontNormal;
	alias Sol = Interface.Solenoids.Index;
	alias Sw = Interface.Switches.Index;
	alias C = Coordinate!();

	mixin Reflexes!() reflexes;
	mixin TextDisplay!() text_display;
	mixin ScoreDisplay!() score_display;

	private {
		Interface iface;
		FontNormal font_normal;

		// Ball return
		bool enable_ball_return;
		KeepValueDelay ball_return;
		Rising dtb_scored;

		// Audio
		//AudioSource score_sound;

		AudioInterface audio_interface;
		Playlist playlist;
		Sound score_sound;
	}

	this(Interface iface) {
		this.iface = iface;
		this.font_normal = new FontNormal(font_5x8_data);

		reflexes.this_();
		text_display.this_();
		score_display.this_();

		this.enable_ball_return = false;

		// Debounce ball out switch in the sense
		// that we want it to be "true" for at least 1000 msecs
		// before reacting so ball has really come to halt
		this.ball_return = KeepValueDelay(
			() => iface.switches[Sw.BALL_OUT], true, 1000.msecs
		);

		this.dtb_scored = Rising(() => iface.switches[Sw.DTB0_0]);

		this.audio_interface = AudioInterface(this.iface.logger);

		this.playlist = new Playlist(
			this.iface.logger, //"./resources/sounds/utini.mp3",
			"./resources/music/original/01_IV_main_theme.mp3",
			"./resources/music/original/02_IV_leias_theme.mp3"
		);
		this.playlist.set_volume(0.7);
		schedule(this.playlist);

		this.score_sound = new Sound(this.iface.logger, "./resources/sounds/blip1_s.mp3");
		schedule(this.score_sound);
	}

	@nogc
	override void frame_start(Duration dt) {
		iface.switches.frame_start(dt);
		reflexes.frame_start_(dt);
		iface.solenoids.frame_start(dt);

		ball_return.frame_start(dt);
		dtb_scored.frame_start(dt);

		assumeNoGC(&check_scoring)(dt);

		iface.canvas.clear;
		text_display.frame_start_(dt);
		score_display.frame_start_(dt);

		audio_interface.frame_start(dt);
	}

	void intro() {
		blank(4200.msecs);
		playlist.play();
		blank(800.msecs);

		text = font_normal("  STAR  \n  WARS  \n\n\n Ep. IV \n\n  A new \n  hope  ", 3);
		text_display.show = true;

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
		text_display.show = true;
		yield(1000.msecs);
		blank();
	}

	override void run() {
		intro();
		iface.logger.log("Game started.");
		enable_ball_return = true;
	}
}


// Some more or less direct switch -> solenoid mappings
mixin template Reflexes() {

	void this_() {
	}

	@nogc
	void frame_start_(Duration dt) {
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
	}
}


// Show a piece of potentially scrolling text on the display
mixin template TextDisplay() {
	StringCanvas!FontNormal text;
	bool show;

	Scrolling!(Interface.Canvas) marquee;
	
	void this_() {
		marquee = new Scrolling!(Interface.Canvas)(
			iface.canvas,
			Coord(80, iface.canvas.size.column),
		);
	}

	@nogc
	void frame_start_(Duration dt) {
		marquee.next_frame(dt);
		if(show) {
			blit(text, Coord(), text.size, marquee, Coord());
		}
	}

	void blank(Duration t = 100.msecs) {
		show = false;
		yield(t);
	}

	void blink(Duration duration = 1000.msecs, Duration interval = 100.msecs) {
		auto t = 0.msecs;
		while(t < duration) {
			show = !show;
			yield(interval);
			t += interval;
		}
	}
}

mixin template ScoreDisplay() {
	size_t score;
	size_t display_score;
	Duration show_score;
	StringCanvas!FontNormal score_text;

	void this_() {
		this.show_score = 0.msecs;
		this.score = 0;
	}

	@nogc
	void frame_start_(Duration dt) {
		if(show_score > 0.msecs) {
			iface.canvas.clear;
			blit_center!(canvas.blit)(score_text, iface.canvas);
		}
	}

	void check_scoring(Duration dt) {
		this.show_score -= dt;
		if(dtb_scored) {
			score_sound.play;
			add_score(100);
		}

		if(display_score < score) {
			display_score += 10;
			render_score();
		}
	}

	void add_score(int score) {
		this.score += score;
		iface.logger.logf("Score: %d", this.score);
		this.show_score = 2000.msecs;
		render_score();
	}

	void render_score() {
		char[10] score_string;
		snprintf(score_string.ptr, score_string.length, "%d", this.display_score);
		this.score_text = font_normal(to!string(score_string.ptr));
	}
}






