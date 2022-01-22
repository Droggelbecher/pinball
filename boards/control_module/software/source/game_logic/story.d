
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

import audio_sdl; // initialize_audio
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
import condition: make_condition, Condition;

import leia_r2d2: leia_r2d2_sprite;
import death_star_reach: death_star_reach_sprite;
import highscore;
import text_entry;

/**
 * Main story flow.
 */
class Story(Interface_) : Task {

	alias Interface = Interface_;

	/// Interface with the user (includes display, solenoid control, inputs)
	Interface iface;

	// Several type aliases for convenience & readability

	alias Font!(font_5x8_size) DefaultFont;
	alias Sw = Interface.Switches.Index;
	alias Lamp = Interface.LEDStripe.Lamp;
	alias C = Coordinate!();
	alias Input = SwitchesAsInput!(iface.Switches);
	alias Command = Input.Command;
	alias Entry = TextEntry!(Interface.Canvas, Input, DefaultFont);
	alias Field = PlayingField!iface;
	alias Text = TextDisplay!iface;
	alias Score = ScoreDisplay!iface;

	// How much various playing field elements will score
	enum SCORE_SPINNER = 10;
	enum SCORE_SLINGSHOT = 10;
	enum SCORE_BUMPER = 10;
	enum SCORE_DTB = 100;
	enum MULTIPLIER_DTB = 2;
	enum SCORE_DTB_ALL = 100;
	enum SCORE_HOLE0 = 1000;

	enum SCORE_PHASE2_REQUIRED = 10000;
	enum SCORE_PHASE2_BONUS = 1000;


	enum RGB {
		BLACK = [0x00, 0x00, 0x00],
		WHITE = [0xf0, 0xf0, 0xf0],

		YELLOW = [0xf0, 0x60, 0x00],
		BLUE = [0x00, 0x00, 0xf0],
		GREEN = [0x20, 0xe0, 0x00],
		RED = [0xf0, 0x00, 0x00],
		ORANGE = [0xf0, 0x30, 0x00],
	};

	alias DColor = Interface.Display.Color;
	alias ColorMode = Interface.LEDStripe.ColorMode;
	alias AnimationMode = Interface.LEDStripe.AnimationMode;

	enum MAX_PLAYERS = 4;

	private {
		DefaultFont default_font;

		/// Audio related

		Playlist playlist;
		SoundRepository sounds;
		// Sounds to play when DTB is scored
		string[] dtb_sounds;
		// Sounds to play on ball out
		string[] fail_sounds;

		Field field;
		Input input;
		Text text;
		Score score;
		Sprite current_animation = null;

		/// Player related

		int n_players = 1;
		int current_player = 0;
		Player[MAX_PLAYERS] players;

		ubyte[3][MAX_PLAYERS] player_rgb; //= [
			//RGB.YELLOW, RGB.GREEN, RGB.RED, RGB.ORANGE
		//];
		DColor[MAX_PLAYERS] player_dcolor = [
			DColor.YELLOW, DColor.GREEN, DColor.RED, DColor.ORANGE
		];

		bool ball_out = false;
	}

	this(Interface iface) {

		player_rgb[0] = RGB.YELLOW;
		player_rgb[1] = RGB.GREEN;
		player_rgb[2] = RGB.RED;
		player_rgb[3] = RGB.ORANGE;

		this.iface = iface;

		this.default_font = new DefaultFont(font_5x8_data);

		//
		// Audio
		//

		initialize_audio();
		this.sounds = new SoundRepository("./resources/sounds/");

		/**
		  Playlist is generally independent from player/game state, start
		  with main theme synced up to STAR WARS prompt
		 */

		this.playlist = new Playlist(
				"./resources/music/original/01_IV_main_theme.mp3",
				//"./resources/music/original/02_IV_leias_theme.mp3"
				"./resources/music/original/09_IV_cantina_band.mp3",
				//"./resources/music/original/03_IV_the_little_people.mp3",
				"./resources/music/original/04_V_imperial_march.mp3",
				"./resources/music/original/05_V_yodas_theme.mp3",
				"./resources/music/original/10_IV_here_they_come.mp3",
				);

		this.playlist.set_volume(0.5);
		this.playlist.set_random(true);
		schedule(this.playlist);

		this.fail_sounds = [
			"obi_du_musst_tun", "vader_beklagenswert"
		];

		// TODO(HeH) some of these are too long, cut or replace!
		this.dtb_sounds = [
			"utini", "r2d2_02", "obi_druiden", "han_rasender_falke", "leia_so_klein"
		];

		this.field = new Field();
		this.field.off;
		schedule(this.field);

		this.input = new Input(iface.switches);
		this.input.off;
		schedule(this.input);

		// text.frame_start() is scheduled manually in our frame_start()
		this.text = new Text();

		// score.frame_start() is scheduled manually in our frame_start()
		this.score = new Score();
		this.score.off;

		for(int i=0; i<MAX_PLAYERS; i++) {
			players[i] = new Player();
		}
	}

	/**
	  Frame start.
	  Clear canvas, run animations, track ball-out state.
	 */
	override void frame_start(Duration dt) {
		iface.canvas.clear;
		text.frame_start(dt);

		if(current_animation) {
			current_animation.frame_start(dt);
			blit(current_animation, Coord(), current_animation.size, iface.canvas, Coord());
		}

		if(!field.enabled) {
			return;
		}

		check_scoring();
		score.frame_start(dt);

		if(field.ball_out()) {
			ball_out = true;
			infof("Player %d ball out at %d balls.", current_player + 1, player.balls);
		}
	}

	/**
	  Return the currently active player.
	 */
	@property
	Player player() {
		return this.players[this.current_player];
	}

	/**
	  Check `field` status for any scoring in the past frame and inform
	  `score` about the added score if any. Also schedules scoring
	  sounds.
	 */
	void check_scoring() {
		if(!field.enabled) {
			return;
		}

		with(field) {
			if(spinner_scored()) {
				sounds.play("spin");
				score.add_score(SCORE_SPINNER);
			}

			if(slingshot_scored()) {
				// TODO(HeH): Separate slingshot sound
				sounds.play("bumper");
				score.add_score(SCORE_SLINGSHOT);
			}

			if(bumper_scored()) {
				sounds.play("bumper");
				score.add_score(SCORE_BUMPER);
			}

			foreach(i, dtb; dtb_scored) {
				if(dtb()) {
					sounds.play(dtb_sounds[i]);
					score.add_score(SCORE_DTB);
					player.increase_multiplier(MULTIPLIER_DTB);
					// DEBUG: toggle DS_WEAPON on score so we know it works
					//iface.led_stripe[Lamp.DS_WEAPON] = !iface.led_stripe[Lamp.DS_WEAPON];
				}
			} // foreach

			if(dtb_all_scored()) {
				sounds.play("score_01");
				score.add_score(SCORE_DTB_ALL);
			}

			if(hole0_hit()) {
				//sounds.play("explode");
				sounds.play("obi_mond_raumstation");
				// TODO(HeH): Maybe "das ist kein mond, das ist eine raumstation"?
				score.add_score(SCORE_HOLE0);

				schedule({
					for(int i = 0; i < 5; i++) {
						iface.led_stripe.lamp(Lamp.DS_LIGHT, true);
						yield(100.msecs);
						iface.led_stripe.lamp(Lamp.DS_LIGHT, false);
						yield(100.msecs);
					}
				});
			}
		} // field
	} // check_scoring()

	/**
	  Display a given string (may contain newlines) with vertical scrolling for
	  the given amount of time.  Will display scrolling lights on the LED
	  stripe in matching speed.
	
	  Args:
		s: Text to display
		color: Text color to use
		speed: Scrolling speed
		initial_wait: Wait this long before starting to scroll
		interruptible: Iff True, abort text display when any flipper is hit
		TODO stripe_color: Color to use for LED stripe
		
	 */
	void scroll_text(string s, DColor color = DColor.YELLOW,
			double speed = 5, Duration initial_wait = 0.msecs, bool interruptible = false)
	{
		auto n = count(s, '\n');
		this.score.off;
		this.text.scroll.reset;
		this.text.s(s, cast(ubyte)color);
		this.text.on;
		this.iface.led_stripe.full(RGB.YELLOW).dt(cast(ubyte)(initial_wait.total!"msecs"));
		
		this.yield(initial_wait);

		//ubyte[3] rgb_color = this.player_rgb[current_player];
		
		this.iface.led_stripe.rotmod(RGB.YELLOW, 3, cast(ubyte)(100 / speed));
		this.text.scroll.speed = Coordinate!double(-speed, 0);
		if(n > 2) {
			// speed is in pixels per sec
			// to display all n+1 lines we need to scroll (n+1-1)*8 pixels
			// which should take (n-1)*8/speed seconds
			// or 8000*(n-1)/speed msecs
			
			// End when we scrolled through
			Condition condition = make_condition(
				msecs(cast(long)(8000.0 * (n - 1) / speed))
			);
			
			if(interruptible) {
				// ... OR a flipper was pressed
				condition = condition | make_condition(
						() => iface.switches[Sw.FLIPPER_LEFT] || iface.switches[Sw.FLIPPER_RIGHT]
						);
			}
			
			this.yield(condition);
		}
		this.text.scroll.stop;
		//this.iface.led_stripe.full(RGB.BLACK);

		// Reset LED stripe to player color

		// Gradient to black
		//this.iface.led_stripe.mode(ColorMode.GRADIENT, AnimationMode.ROTATE);
		//this.iface.led_stripe.color0(RGB.BLACK);
		//this.iface.led_stripe.color1(cast(ubyte[3])(this.player_rgb[current_player]));
		//this.iface.led_stripe.dt(100);
		//this.iface.led_stripe.mod(1);

		this.iface.led_stripe.rotmod(cast(ubyte[3])(this.player_rgb[current_player]), 6, 60);

		this.text.off;
		this.score.on;
	}

	/**
	  Show current highscore and "STAR WARS" intro text
	 */
	void intro() {
		this.text.off;
		this.playlist.play;

		// This is timed to the wav file
		this.yield(800.msecs);

		string intro_text = "  STAR\n  WARS\n\n\n\n";
		foreach(hs; highscore.top(5)) {
			intro_text ~= format!"%s\n%8d\n"(hs[0], hs[1]);
		}
		intro_text ~= "\n\n\n\n";

		this.scroll_text(intro_text, DColor.YELLOW, 10, 2000.msecs, true);
	}

	/**
	  Ask user for player count.
	  Sets `this.n_players` and `this.current_player`.
	 */
	void choose_player_count() {
		// Turn off playing field, use flippers as data input
		this.field.off;
		this.input.on;
		this.text.scroll.reset;

		while(true) {
			this.text.s(format!"   %d\n Player%s"(n_players, n_players > 1 ? "s" : ""), DColor.GREEN);
			blink_text(100.msecs);

			Command c = input.pop_command;

			if(c == Command.NEXT) {
				sounds.play("select");
				n_players++;
				if(n_players > MAX_PLAYERS) { n_players = 1; }
			}
			else if(c == Command.SELECT) {
				sounds.play("select2");
				yield(200.msecs);
				break;
			}
		}

		for(int i=0; i<n_players; i++) {
			players[i] = new Player;
		}

		current_player = 0;
	}

	/**
	  Story/game sequence for a single player
	 */
	void player_go(int n) {
		field.reset;

		{
			score.off;
			scope(exit) score.on;

			iface.logger.logf("Player %d's turn.", n + 1);

			current_player = n;
			player.reset();
			score.player = player;
			score.color = player_dcolor[current_player];

			field.return_ball;
			ball_out = false;

			scroll_text(format!"\n\nPLAYER %d\n\n %2d\x03  \n\nMAY THE\n FORCE  \nBE WITH\n  YOU \n\n\n"(current_player + 1, player.balls), player_dcolor[current_player], 10.0);

			sounds.play("start");
		}

		// Phase I
		// - Goal: Get to score X
		// - Theme: Main Theme
		// - Effects: Falcon
		// - Drop targets -> R2D2/Utini/etc... -> Multiplier
		// (- Alle Drop targets: Cantina Band special mit Multiball?)
		// -> "Helft mir Obi Wan Kenobi, ihr seid meine letzte Hoffnung"

		if(player.score < SCORE_PHASE2_REQUIRED) {

			yield(() => player.score >= SCORE_PHASE2_REQUIRED || ball_out);
			if(ball_out) { return; }

			// Leia Sequence
			{
				yield(2000.msecs);

				score.off;
				scope(exit) score.on;

				yield(playlist.fade_out);
				scope(exit) playlist.play;

				current_animation = leia_r2d2_sprite;
				scope(exit) current_animation = null;

				// TODO: Maybe use shorter variant here?
				sounds.play("leia_helft_mir_kurz");
				yield(4000.msecs);
			}
			scroll_text("\n\n Enter  \n  the   \n Death  \n Star!\n\n  +3\x03\n\n\n\n", DColor.GREEN,
					10.0);
			score.add_score(SCORE_PHASE2_BONUS);
			player.balls = player.balls + 3;
		}


		// Phase II: Inside Death star
		// - Goal: Free leia & disable energy (death star hard door & drop targets)
		// - Theme: Imperial March
		// - Effects: Darth Vader
		// - Sounds: Darth Vader breathing, Leia?, Muellschlucker?, Stromtrooper/laser, lichtschwertkampf, obi/darth
		// - Bonus: Kill X stormtroopers (spinner)

		// TODO: Vader breathing
		iface.led_stripe.lamp(Lamp.VADER, true);

		yield(() => field.hole0_hit() || ball_out);
		if(ball_out) { return; }

		sounds.play("obi_mond_raumstation");
		score.add_score(10000);

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
					field.hole0_hit()
					|| ball_out
					|| field.dtb_all_scored()
				 );
			if(ball_out) { return; }

			if(field.hole0_hit()) {
				sounds.play("explode");
			}

			score.add_score(10000);
		}
	}

	/**
	  Return true iff the game is over.
	 */
	bool lost_ball() {
		sounds.play("failure");
		yield(1000.msecs);
		sounds.play(choice(fail_sounds));

		score.off;
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
		score.on;
		return false;
	}

	/**
	  Display `this.score` in with blinking interval `interval` for a total of `duration`.
	 */
	void blink_text(Duration duration = 1000.msecs, Duration interval = 100.msecs) {
		auto score_bak = score.enabled();
		score.off;
		auto t = 0.msecs;
		while(t < duration) {
			text.toggle;
			yield(interval);
			t += interval;
		}
		score.on(score_bak);
	}

	void enter_highscore() {
		this.field.off;
		this.text.scroll.reset;
		this.yield(this.playlist.fade_out());

		this.sounds.play("lorbeeren");

		//this.text_entry.on;

		// TODO: FIXME: seems this doesnt work and leads to and endless busy loop
		for(int p = 0; p < n_players; p++) {
			//this.input.on;
			//this.text_entry.on;
			//this.text_entry.reset;

			auto text_entry = new Entry(iface.canvas, input, default_font, MAX_PLAYERS);
				//new TextEntry!(Interface.Display, Input)(iface.display, input, 4);
			text_entry.on;
			schedule(text_entry);
			yield(() => !text_entry.running);
			highscore.add_entry(text_entry.value, players[p].score);
			//append("scores.csv", format!"%s,%d\n"(text_entry.value, players[p].score));


			//char[4] name = "AAAA";
			//int pos = 0;

			/+
			while(true) {
				// TODO: change this to two blits
				this.text.s(format!"Player %d\n  %s"(p+1), DColor.YELLOW);
				this.text.s(format!"  %s"(name), DColor.YELLOW);
				//text.s(format!"\n  %s"(name), DColor.GREEN);
				yield(100.msecs);

				Command c = input.query;
				if(c == Command.NEXT) {
					sounds.play("select");
					name[pos]++;
					if(name[pos] > 'Z') {
						name[pos] = 'A';
					}
				}
				else if(c == Command.SELECT) {
					sounds.play("select2");
					pos++;
					if(pos >= 4) {
						// TODO: Actually save high score somewhere?
						append("scores.csv", format!"%s,%d\n"(name, players[p].score));
						break;
					}
				}
			} // while
			+/
		} // for

	}

	override void run() {

		while(true) {

			iface.logger.log("Starting intro");
			intro();
			iface.logger.log("Choosing player count");
			choose_player_count();

			iface.logger.log("Game started.");
			field.on;
			score.on;

			player_go(0);

			while(true) {
				if(ball_out) {
					if(lost_ball()) {
						// is the game over?
						break;
					}
					player_go(current_player);
				}
				yield(() => ball_out);
			}

			field.off;
			score.off;
			text.off;

			enter_highscore();


		}

	}
}

