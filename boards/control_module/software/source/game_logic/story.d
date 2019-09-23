
import task: Task;
import std.datetime: Duration, seconds, msecs;

import std.experimental.logger;
import std.stdio:    writeln;
import std.conv;
import std.typecons;
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

import text_display;
import score_display;
import playing_field;
//import read_png;
import sprite;

alias Font!(font_5x8_size) FontNormal;

class Story(Interface_) : Task {

	alias Interface = Interface_;
	alias Sw = Interface.Switches.Index;
	alias C = Coordinate!();

	enum {
		YELLOW = [0xf0, 0x60, 0x00],
		BLACK = [0x00, 0x00, 0x30],
	};

	Interface iface;

	private {
		AudioInterface audio_interface;
		Playlist playlist;
		Sound score_sound;
		Sound score_sound_2;

		PlayingField!iface playing_field;
		TextDisplay!iface text;
		ScoreDisplay!iface score_display;

		//PNGSprite!(Interface.Canvas) animation;
	}

	this(Interface iface) {
		this.iface = iface;

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
		this.playing_field.off;
		schedule(this.playing_field);

		this.text = new TextDisplay!(this.iface)();
		schedule(this.text, priority + 10);

		this.score_display = new ScoreDisplay!(this.iface)();
		schedule(this.score_display, priority + 20);

		//this.animation = new PNGSprite!(Interface.Canvas)(
				//"./resources/sprites/deathstar_01",
				//300.msecs,
				//this.iface.canvas,
				//Yes.loop
		//);
		//schedule(this.animation, priority + 5);
	}

	override void frame_start(Duration dt) {
		iface.canvas.clear;

		//this.animation.frame_start(dt);

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

	//void intro() {
		//text.off;
		////auto png_buf = read_png("/home/henning/host/Desktop/test.png");
		////blit(png_buf, Coord(0, 0), Coord(16, 40), iface.canvas, Coord(0,0));
	//}

	void intro() {
		text.off;
		yield(4200.msecs);
		playlist.play;
		yield(800.msecs);

		text.s("  STAR  \n  WARS  \n\n\n Ep. IV \n\n  A new \n  hope  ", 3);
		text.on;
		iface.led_stripe.full(YELLOW).dt(10);

		yield(2000.msecs);
		text.scroll.speed = Coordinate!double(-5, 0);
		iface.led_stripe.rotmod(YELLOW, 5, 100);

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
		text.on;
		yield(1000.msecs);
		text.off;
	}

	void blink_text(Duration duration = 1000.msecs, Duration interval = 100.msecs) {
		auto t = 0.msecs;
		while(t < duration) {
			yield(interval);
			t += interval;
		}
	}

	override void run() {
		intro();
		iface.logger.log("Game started.");
		playing_field.on;
	}
}

