
import task: Task;
import std.datetime: Duration, seconds, msecs;

import std.experimental.logger;
import std.stdio:    writeln;
import std.conv;
import std.algorithm: count;
import std.typecons;
import core.stdc.stdio;
import std.format;
import std.random: choice;

import audio_sdl;
import canvas:       blit, blit_center, clear, set_color;
import canvas;
import coordinate;
import five_eight:   font_5x8_size, font_5x8_data;
import font:         Font, StringCanvas;
import scrolling;
import scrolling:    blit;

import text_display;
import score_display;
import playing_field;
import switches_as_input;
import sprite: Sprite;
//import img_canvas: ImgCanvas;
//import string_canvas: string_to_canvas, test_animation;
import player: Player;

import leia_r2d2: leia_r2d2_sprite;
import death_star_reach: death_star_reach_sprite;

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
		SoundRepository sounds;
		string[] dtb_sounds;
		string[] fail_sounds;

		PlayingField!iface playing_field;
		SwitchesAsInput!iface switches_as_input;
		TextDisplay!iface text;
		ScoreDisplay!iface score_display;
		Sprite current_animation = null;

		int n_players = 1;
		int current_player = 0;
		Player[MAX_PLAYERS] players;
		bool ball_out = false;
	}

	this(Interface iface) {
		this.iface = iface;

		//
		// Audio
		//

		this.audio_interface = new AudioInterface();
		this.sounds = new SoundRepository("./resources/sounds/");

		/**
			TODO: How do we want to manage playlist in particular the main theme?

			a) playlist is generally independent from player/game state, start with main theme synced up to STAR WARS prompt
			b) playlist per player state, move the "STAR WARS" into player init
			c) playlist per player state, use main theme only for STAR WARS intro and maybe later
		*/

		this.playlist = new Playlist(
			"./resources/music/original/01_IV_main_theme.mp3",
			//"./resources/music/original/02_IV_leias_theme.mp3"
			"./resources/music/original/09_IV_cantina_band.mp3",
			//"./resources/music/original/03_IV_the_little_people.mp3",
			"./resources/music/original/04_V_imperial_march.mp3",
			"./resources/music/original/05_V_yodas_theme.mp3",
			"./resources/music/original/10_IV_here_they_come.mp3",
			// 03 the little people
		);

		this.playlist.set_volume(0.5);
		this.playlist.set_random(true);
		schedule(this.playlist);

		this.fail_sounds = [
			"obi_du_musst_tun", "vader_beklagenswert"
		];

		this.dtb_sounds = [
			"utini", "r2d2_02", "obi_druiden", "han_rasender_falke", "leia_so_klein"
		];

		this.playing_field = new PlayingField!(this.iface)();
		this.playing_field.off;
		schedule(this.playing_field);

		this.switches_as_input = new SwitchesAsInput!(this.iface)();
		this.switches_as_input.off;
		schedule(this.switches_as_input);

		this.text = new TextDisplay!(this.iface)();

		this.score_display = new ScoreDisplay!(this.iface)();
		this.score_display.off;

		for(int i=0; i<MAX_PLAYERS; i++) {
			players[i] = new Player();
		}
	}

	override void frame_start(Duration dt) {
		iface.canvas.clear;
		text.frame_start(dt);

		if(current_animation) {
			current_animation.frame_start(dt);
			blit(current_animation, Coord(), current_animation.size, iface.canvas, Coord());
		}

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
				sounds.play("spin");
				score_display.add_score(1);
			}

			if(slingshot_scored()) {
				sounds.play("bumper");
				score_display.add_score(10);
			}

			if(bumper_scored()) {
				sounds.play("bumper");
				score_display.add_score(10);
			}

			foreach(i, dtb; dtb_scored) {
				if(dtb()) {
					sounds.play(dtb_sounds[i]);
					score_display.add_score(100);
					player.increase_multiplier(2);
					// DEBUG: toggle DS_WEAPON on score so we know it works
					//iface.led_stripe[Lamp.DS_WEAPON] = !iface.led_stripe[Lamp.DS_WEAPON];
				}
			} // foreach
			if(dtb_all_scored()) {
				sounds.play("score_01");
				score_display.add_score(100);
			}

			if(hole0_hit()) {
				//sounds.play("explode");
				score_display.add_score(1000);
				
				schedule({
					for(int i = 0; i < 5; i++) {
						iface.led_stripe.lamp(Lamp.DS_LIGHT, true);
						yield(100.msecs);
						iface.led_stripe.lamp(Lamp.DS_LIGHT, false);
						yield(100.msecs);
					}
				});
			}
		} // playing_field
	} // check_scoring()

	/**
	  Display a given string (may contain newlines) with vertical scrolling for the given amount of time.
	  Will display scrolling lights on the LED stripe in matching speed and color.
	*/
	void scroll_text(string s, DColor color = DColor.YELLOW, double speed = 5, Duration initial_wait = 0.msecs) {
		auto n = count(s, '\n');
		score_display.off;
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
		score_display.on;
	}

	void intro() {
		text.off;
		//yield(4200.msecs);
		playlist.play;
		yield(800.msecs);

		scroll_text("  STAR\n  WARS\n\n\n\n", DColor.YELLOW, 5, 2000.msecs);
	}

	/**
	  Ask user for player count.
	  Sets `this.n_players` and `this.current_player`.
	*/
	void choose_player_count() {
		playing_field.off;
		switches_as_input.on;
		text.scroll.reset;

		while(true) {
			text.s(format!"   %d\n Player%s"(n_players, n_players > 1 ? "s" : ""), DColor.GREEN);
			blink_text(100.msecs);

			Command c = switches_as_input.query;

			if(c == Command.NEXT) {
				sounds.play("select");
				n_players++;
				if(n_players > MAX_PLAYERS) { n_players = 1; }
			}
			else if(c == Command.SELECT) {
				break;
			}
		}

		current_player = 0;
	}

	/**
	  Story/game sequence for a single player
	 */
	void player_go(int n) {
		// TODO: Reset playfield, eg. DTBs
		playing_field.reset;

		{
			score_display.off;
			scope(exit) score_display.on;

			iface.logger.logf("Player %d's turn.", n + 1);

			current_player = n;
			player.reset();
			score_display.player = player;

			scroll_text(format!"\n\nPLAYER %d\n\n %2d\x03  \n\nMAY THE\n FORCE  \nBE WITH\n  YOU  \n\n\n"(current_player + 1, player.balls), DColor.YELLOW, 10.0);

			ball_out = false;
			playing_field.return_ball;
			sounds.play("start");
		}

		// Phase I
		// - Goal: Get to score X
		// - Theme: Main Theme
		// - Effects: Falcon
		// - Drop targets -> R2D2/Utini/etc... -> Multiplier
		// (- Alle Drop targets: Cantina Band special mit Multiball?)
		// -> "Helft mir Obi Wan Kenobi, ihr seid meine letzte Hoffnung"

		if(player.score < 17000) {

			yield(() => player.score >= 17000 || ball_out);
			if(ball_out) { return; }

			// Leia Sequence
			{
				score_display.off;
				scope(exit) score_display.on;

				yield(playlist.fade_out);
				scope(exit) playlist.play;

				current_animation = leia_r2d2_sprite;
				scope(exit) current_animation = null;

				// TODO: Maybe use shorter variant here?
				sounds.play("leia_helft_mir_kurz");
				yield(4000.msecs);
			}
			score_display.add_score(10000);
		}

		
		// Phase II: Inside Death star
		// - Goal: Free leia & disable energy (death star hard door & drop targets)
		// - Theme: Imperial March
		// - Effects: Darth Vader
		// - Sounds: Darth Vader breathing, Leia?, Muellschlucker?, Stromtrooper/laser, lichtschwertkampf, obi/darth
		// - Bonus: Kill X stormtroopers (spinner)

		// TODO: Vader breathing
		iface.led_stripe.lamp(Lamp.VADER, true);

		yield(() => playing_field.hole0_hit() || ball_out);
		if(ball_out) { return; }

		sounds.play("obi_mond_raumstation");
		score_display.add_score(10000);

		// Phase III: Angriff auf Todesstern
		// - Goal:
		//     1. Tuerme ausschalten (drop targets)
		//     2. Death star (easy door)
		// - Theme: Here they come
		// - Sounds: Weiter aufs Ziel zu, Vertraue der Macht
		// - Effects: Target leds

		// TODO: Death star closing in sequence animation

		while(true) {
			// TODO: sound: "weiter aufs ziel zu..."
			// TODO: sound: "vertraue der macht"
			iface.led_stripe.lamp(Lamp.VADER, false);
			iface.led_stripe.lamp(Lamp.TARGET, true);

			yield(() =>
					   playing_field.hole0_hit()
					|| ball_out
					|| playing_field.dtb_all_scored()
			);
			if(ball_out) { return; }

			if(playing_field.hole0_hit()) {
				sounds.play("explode");
			}

			score_display.add_score(10000);
		}
	}

	/**
		Return: true iff game is over.
	*/
	bool lost_ball() {
		sounds.play("failure");
		yield(1000.msecs);
		//sounds.play("vader_beklagenswert");
		sounds.play(choice(fail_sounds));

		score_display.off;
		text.scroll.reset;
		text.on;
		text.s(format!"PLAYER %d\n %2d\x03"(current_player + 1, player.balls), DColor.GREEN);
		yield(1000.msecs);
		player.balls = player.balls - 1;
		text.s(format!"PLAYER %d\n %2d\03"(current_player + 1, player.balls), DColor.RED);
		blink_text(1000.msecs);
		yield(1000.msecs);

		current_player++;
		current_player %= n_players;

		auto full_circle = current_player;

		while(players[current_player].balls == 0) {
			current_player++;
			current_player %= n_players;
			if(current_player == full_circle) {
				return true;
			}
		}
		
		text.off;
		score_display.on;
		return false;
	}

	void blink_text(Duration duration = 1000.msecs, Duration interval = 100.msecs) {
		auto score_display_bak = score_display.enabled();
		score_display.off;
		auto t = 0.msecs;
		while(t < duration) {
			text.toggle;
			yield(interval);
			t += interval;
		}
		score_display.on(score_display_bak);
	}

	void highscore() {
		playing_field.off;
		switches_as_input.on;
		text.scroll.reset;
		yield(playlist.fade_out());

		sounds.play("lorbeeren");

		// TODO: FIXME: seems this doesnt work and leads to and endless busy loop
		for(int p = 0; p < n_players; p++) {
			char[4] name = "AAAA";
			int pos = 0;

			while(true) {
				// TODO: change this to two blits
				text.s(format!"Player %d"(p), DColor.YELLOW);
				text.s(format!"\n  %s"(name), DColor.GREEN);
				yield(100.msecs);

				Command c = switches_as_input.query;
				if(c == Command.NEXT) {
					sounds.play("select");
					name[pos]++;
					if(name[pos] > 'Z') {
						name[pos] = 'A';
					}
				}
				else if(c == Command.SELECT) {
					pos++;
					if(pos >= 4) {
						// TODO: Actually save high score somewhere?
						break;
					}
				}
			} // while
		} // for

	}

	override void run() {

		iface.logger.log("Starting intro");
		intro();
		iface.logger.log("Choosing player count");
		choose_player_count();

		iface.logger.log("Game started.");
		playing_field.on;
		score_display.on;

		//player_go(0);

		//while(true) {
			//if(ball_out) {
				//if(lost_ball()) {
					//// is the game over?
					//break;
				//}
				//player_go(current_player);
			//}
			//yield(() => ball_out);
		//}

		highscore();

	}
}

