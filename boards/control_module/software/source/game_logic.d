
import task: Task;
import std.datetime: Duration, seconds, msecs;

import std.experimental.logger;
import utils: assumeNoGC;
import std.stdio:    writeln;
import std.conv;
import core.stdc.stdio;

import audio_sdl;
import canvas:       blit, blit_center, clear, set_color;
import canvas;
import coordinate;
import five_eight:   font_5x8_size, font_5x8_data;
import font:         Font, StringCanvas;
import scrolling;
import scrolling:    blit;
import signal;

alias Font!(font_5x8_size) FontNormal;

class GameLogic(Interface_) : Task {

	alias Interface = Interface_;
	alias Sw = Interface.Switches.Index;
	alias C = Coordinate!();

	enum {
		YELLOW = [0xf0, 0x60, 0x00],
		BLACK = [0x00, 0x00, 0x30],
	};

	private {
		Interface iface;
		//FontNormal font_normal;

		// Ball return

		AudioInterface audio_interface;
		Playlist playlist;
		Sound score_sound;
		Sound score_sound_2;

		PlayingField!iface playing_field;
		TextDisplay!iface text;
		ScoreDisplay!iface score_display;
	}

	this(Interface iface) {
		this.iface = iface;
		//this.font_normal = new FontNormal(font_5x8_data);

		this.audio_interface = new AudioInterface();

		this.playlist = new Playlist(
			"./resources/music/original/01_IV_main_theme.mp3",
			"./resources/music/original/02_IV_leias_theme.mp3"
		);
		this.playlist.set_volume(0.7);
		schedule(this.playlist);

		this.score_sound = new Sound("./resources/sounds/blip1_s.mp3");
		this.score_sound_2 = new Sound("./resources/sounds/utini.mp3");

		this.playing_field = new PlayingField!(this.iface)();
		schedule(this.playing_field);

		this.text = new TextDisplay!(this.iface)();
		schedule(this.text, priority + 10);

		this.score_display = new ScoreDisplay!(this.iface)();
		schedule(this.score_display, priority + 20);
	}

	override void frame_start(Duration dt) {
		iface.canvas.clear;
		check_scoring();
	}

	void check_scoring() {
		with(playing_field) {
			foreach(dtb; dtb_scored) {
				if(dtb) {
					score_sound.play;
					score_display.add_score(100);
				}
			} // foreach
			if(dtb_all_scored) {
				score_sound_2.play;
				score_display.add_score(1000);
			}
		} // playing_field
	} // check_scoring()

	void intro() {
		text.off;
		yield(4200.msecs);
		playlist.play;
		yield(800.msecs);

		text.s("  STAR  \n  WARS  \n\n\n Ep. IV \n\n  A new \n  hope  ", 3);
		text.on;
		iface.led_stripe.full(YELLOW).dt(10); // module side fading too slow 
		// TODO: implement control module side fading

		yield(2000.msecs);
		text.scroll.speed = Coordinate!double(-5, 0);
		iface.led_stripe.rotmod(YELLOW, 5, 100); // also too slow?!

		yield(9700.msecs);
		text.scroll.stop;
		yield(4000.msecs);
		iface.led_stripe.full(BLACK);
		text.off;
		yield(4000.msecs);

		text.scroll.reset;
		text.s(" READY  \nPLAYER 1", 2);
		yield(1000.msecs);
		blink_text(500.msecs);
		text.enable = true;
		yield(1000.msecs);
		text.off;
	}

	void blink_text(Duration duration = 1000.msecs, Duration interval = 100.msecs) {
		auto t = 0.msecs;
		while(t < duration) {
			text.enable = !text.enable;
			yield(interval);
			t += interval;
		}
	}

	override void run() {
		intro();
		iface.logger.log("Game started.");
		playing_field.enable_ball_return = true;
	}
}

class PlayingField(alias iface) : Task {

	import signal;

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

class TextDisplay(alias iface): Task {

	alias blit = canvas.blit;
	alias blit = scrolling.blit;
	alias Font!(font_5x8_size) FontNormal;
	alias Interface = typeof(iface);

	StringCanvas!FontNormal text;
	bool enable;
	FontNormal font_normal;

	Scrolling!(Interface.Canvas) scroll;

	this() {
		scroll = new Scrolling!(Interface.Canvas)(
			iface.canvas,
			Coord(80, iface.canvas.size.column)
		);
		font_normal = new FontNormal(font_5x8_data);
		enable = false;
	}

	void s(string t, ubyte color = 1) {
		this.text = font_normal(t, color);
	}

	void off() {
		this.enable = false;
	}

	void on() {
		this.enable = true;
	}

	override
	void frame_start(Duration dt) {
		scroll.next_frame(dt);
		if(enable) {
			blit(text, Coord(), text.size, scroll, Coord());
		}
	}

	//void blank(Duration t = 100.msecs) {
		//enable = false;
		//yield(t);
	//}

	//void blink(Duration duration = 1000.msecs, Duration interval = 100.msecs) {
		//auto t = 0.msecs;
		//while(t < duration) {
			//enable = !enable;
			//yield(interval);
			//t += interval;
		//}
	//}
}

class ScoreDisplay(alias iface): Task {
	alias blit = canvas.blit;
	alias blit = scrolling.blit;
	alias Font!(font_5x8_size) FontNormal;
	alias Interface = typeof(iface);

	size_t score;
	size_t display_score;
	Duration show_score;
	StringCanvas!FontNormal score_text;
	FontNormal font_normal;

	this() {
		this.show_score = 0.msecs;
		this.score = 0;
		this.font_normal = new FontNormal(font_5x8_data);
	}

	override
	void frame_start(Duration dt) {
		this.show_score -= dt;
		if(display_score < score) {
			display_score += cast(int)(
				100.0 * (score - display_score + 10) / (this.show_score.total!"msecs" + 10.0)
			);
			if(display_score > score) {
				display_score = score;
			}
			render_score();
		}
		if(show_score > 0.msecs) {
			iface.canvas.clear;
			blit_center!(canvas.blit)(score_text, iface.canvas);
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

