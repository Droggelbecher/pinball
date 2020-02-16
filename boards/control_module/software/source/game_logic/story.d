
import task: Task;
import std.datetime: Duration, seconds, msecs;

import std.experimental.logger;
import std.stdio:    writeln;
import std.conv;
import std.algorithm: count;
import std.typecons;
import core.stdc.stdio;
import std.format;

import audio_sdl;
import canvas:       blit, blit_center, clear, set_color;
import canvas;
import coordinate;
import five_eight:   font_5x8_size, font_5x8_data;
import font:         Font, StringCanvas;
import scrolling;
import scrolling:    blit;
//import signal;

import text_display;
import score_display;
import playing_field;
import switches_as_input;
//import read_png;
import sprite;
import player: Player;

alias Font!(font_5x8_size) FontNormal;

class Story(Interface_) : Task {

	alias Interface = Interface_;
	alias Sw = Interface.Switches.Index;
	alias Lamp = Interface.LEDStripe.Lamp;
	alias C = Coordinate!();
	alias Command = SwitchesAsInput!iface.Command;

	enum RGB {
		YELLOW = [0xf0, 0x60, 0x00],
		BLACK = [0x20, 0x20, 0x20],
	};

	alias DColor = Interface.Display.Color;

	enum MAX_PLAYERS = 4;

	Interface iface;

	private {
		AudioInterface audio_interface;
		Playlist playlist;
		Sound score_sound;
		Sound score_sound_2;
		Sound explode_sound;
		Sound bumper_sound;
		Sound[] character_sounds;
		Sound[] dtb_sounds;
		Sound failure_sound;
		Sound start_sound;
		Sound select_sound;
		Sound spin_sound;

		PlayingField!iface playing_field;
		SwitchesAsInput!iface switches_as_input;
		TextDisplay!iface text;
		ScoreDisplay!iface score_display;

		int n_players = 1;
		int current_player = 0;
		Player[MAX_PLAYERS] players;
		bool ball_out = false;
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
		this.explode_sound = new Sound("./resources/sounds/death_star_explode.mp3");

		this.character_sounds = [
			new Sound("./resources/sounds/utini.mp3"),
			new Sound("./resources/sounds/R3D4.mp3"),
			new Sound("./resources/sounds/utini.mp3"),
			new Sound("./resources/sounds/R3D4.mp3"),
			new Sound("./resources/sounds/utini.mp3"),
			// TODO: C3PO
			// TODO: Obi Wan
			// TODO: ???
		];
		this.dtb_sounds = character_sounds;

		this.bumper_sound = new Sound("./resources/sounds/short/blop1.mp3");
		this.failure_sound = new Sound("./resources/sounds/loss/failure_sound.mp3");
		this.start_sound = new Sound("./resources/sounds/beam_me_up_2.mp3");
		this.select_sound = new Sound("./resources/sounds/short/swirl2.mp3");
		this.spin_sound = new Sound("./resources/sounds/short/swirl3.mp3");

		this.playing_field = new PlayingField!(this.iface)();
		this.playing_field.off;
		schedule(this.playing_field);

		this.switches_as_input = new SwitchesAsInput!(this.iface)();
		this.switches_as_input.off;
		schedule(this.switches_as_input);

		this.text = new TextDisplay!(this.iface)();
		schedule(this.text, priority + 10);

		this.score_display = new ScoreDisplay!(this.iface)();
		//schedule(this.score_display, priority - 20);
		this.score_display.off;

		for(int i=0; i<MAX_PLAYERS; i++) {
			players[i] = new Player();
		}
	}

	override void frame_start(Duration dt) {
		iface.canvas.clear;

		if(!playing_field.enabled) {
			return;
		}

		check_scoring();

		score_display.frame_start(dt);

		if(playing_field.ball_out()) {
			ball_out = true;
			infof("Player %d ball out at %d balls.", current_player + 1, player.balls);
		}
	}

	@property
	Player player() {
		return this.players[this.current_player];
	}

	void check_scoring() {
		if(!playing_field.enabled) {
			return;
		}
		
		with(playing_field) {
			if(spinner_scored()) {
				spin_sound.play;
				score_display.add_score(10);
			}

			if(bumper_scored()) {
				bumper_sound.play;
				score_display.add_score(100);
			}

			foreach(i, dtb; dtb_scored) {
				if(dtb()) {
					dtb_sounds[i].play;
					score_display.add_score(1000);
					player.increase_multiplier(2);
					// DEBUG: toggle DS_WEAPON on score so we know it works
					//iface.led_stripe[Lamp.DS_WEAPON] = !iface.led_stripe[Lamp.DS_WEAPON];
				}
			} // foreach
			if(dtb_all_scored()) {
				score_sound_2.play;
				score_display.add_score(1000);
			}

			if(hole0_hit()) {
				explode_sound.play();
				score_display.add_score(10000);
				iface.led_stripe.lamp(Lamp.DS_LIGHT);
			}
		} // playing_field
	} // check_scoring()

	void scroll_text(string s, DColor color = DColor.YELLOW, double speed = 5, Duration initial_wait = 0.msecs) {
		auto n = count(s, '\n');
		text.scroll.reset;
		text.s(s, cast(ubyte)color);
		text.on;
		iface.led_stripe.full(RGB.YELLOW).dt(cast(ubyte)(50 / speed));
		yield(initial_wait);
		text.scroll.speed = Coordinate!double(-speed, 0);
		if(n > 2) {
			yield(cast(int)(8000.0 * (n - 1) / speed) * 1.msecs);
		}
		text.scroll.stop;
		iface.led_stripe.full(RGB.BLACK);
		text.off;
	}

	void intro() {
		text.off;
		//yield(4200.msecs);
		playlist.play;
		yield(800.msecs);

		scroll_text("  STAR\n  WARS\n\n\n\n", DColor.YELLOW, 5, 2000.msecs);
	}

	void choose_player_count() {
		playing_field.off;
		switches_as_input.on;
		text.scroll.reset;

		text.on;
		while(true) {
			text.s(format!"   %d\n Player%s"(n_players, n_players > 1 ? "s" : ""), DColor.GREEN);
			blink_text(100.msecs);

			Command c = switches_as_input.query;

			if(c == Command.NEXT) {
				select_sound.play;
				n_players++;
				if(n_players > MAX_PLAYERS) { n_players = 1; }
			}
			else if(c == Command.SELECT) {
				break;
			}
		}

		current_player = 0;
	}

	void player_go(int n) {
		iface.logger.logf("Player %d's turn.", n + 1);
		current_player = n;
		player.reset();
		score_display.player = player;

		scroll_text(format!"\n\nPLAYER %d\n\n %2d\x03  \n\nMAY THE\n FORCE  \nBE WITH\n  YOU  \n\n\n"(current_player + 1, player.balls), DColor.YELLOW, 10.0);

		ball_out = false;
		playing_field.return_ball;
		start_sound.play;


		scroll_text("\n\n Reach  \n 10000 $", DColor.YELLOW, 10.0);
		text.on;
		blink_text(1000.msecs);
		text.off;
		//scroll_text("1\n2\n3\n4\n5\n");
		//scroll_text("A\nB\nC\nD\nE\n");

		// Story

		// Phase I
		// - Goal: Get to score X
		// - Theme: Main Theme
		// - Effects: Falcon
		// - Drop targets -> R2D2/Utini/etc... -> Multiplier
		// (- Alle Drop targets: Cantina Band special mit Multiball?)
		// -> "Helft mir Obi Wan Kenobi, ihr seid meine letzte Hoffnung"

		// Phase Ia
		// - Goal: Death star (easy door)
		// -> "Das ist kein Mond, das ist eine Raumstation"

		// Phase II: Inside Death star
		// - Goal: Free leia & disable energy (death star hard door & drop targets)
		// - Theme: Imperial March
		// - Effects: Darth Vader
		// - Sounds: Darth Vader breathing, Leia?, Muellschlucker?, Stromtrooper/laser, lichtschwertkampf, obi/darth
		// - Bonus: Kill X stormtroopers (spinner)

		// Phase III: Angriff auf Todesstern
		// - Goal:
		//     1. Tuerme ausschalten (drop targets)
		//     2. Death star (easy door)
		// - Theme: Here they come
		// - Sounds: Weiter aufs Ziel zu, Vertraue der Macht
		// - Effects: Target leds

	}

	void lost_ball() {
		failure_sound.play;

		text.scroll.reset;
		text.on;
		text.s(format!"PLAYER %d\n %2d\x03"(current_player + 1, player.balls), DColor.GREEN);
		yield(1000.msecs);
		player.balls = player.balls - 1;
		text.s(format!"PLAYER %d\n %2d\03"(current_player + 1, player.balls), DColor.RED);
		blink_text(1000.msecs);
		yield(1000.msecs);

		current_player++;
		if(current_player >= n_players) {
			current_player = 0;
		}
		player_go(current_player);
		text.off;
	}

	void blink_text(Duration duration = 1000.msecs, Duration interval = 100.msecs) {
		auto t = 0.msecs;
		while(t < duration) {
			text.toggle;
			yield(interval);
			t += interval;
		}
	}

	override void run() {

		iface.logger.log("Starting intro");
		intro();
		iface.logger.log("Choosing player count");
		choose_player_count();

		iface.logger.log("Game started.");
		playing_field.on;

		player_go(0);

		score_display.on;
		while(true) {
			if(ball_out) {
				score_display.off;
				lost_ball();
				score_display.on;
			}
			yield(100.msecs);
		}
		//for(int i=0; i<players; i++) {
			//player_go(i);
		//}

	}
}

