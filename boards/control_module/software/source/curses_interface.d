
import std.stdio;
import std.datetime;
import std.string;

import deimos.ncurses;
package alias deimos.ncurses nc;

import buffer_canvas;
import coordinate;
import task;
import solenoids;
import switches;
import sensor_actuator_override;
import buffer_logger;

alias SolenoidsIndex Sol;

struct SolenoidState {
	int key;
	string str;
	Sol index;
}

alias SwitchesIndex Sw;

struct Key {
	int toggle;
	int flip;
	string str;
	Sw index;
	bool state;
}


class CursesInterface(Solenoids_, Switches_) : Task {

	alias SensorActuatorOverride!Solenoids_ Solenoids;
	alias SensorActuatorOverride!Switches_ Switches;
	alias BufferCanvas Canvas;
	alias BufferLogger!(10) Logger;

public:
	// Solenoids
	enum enable_solenoid_control_key = KEY_F(1);
	enum quit_key = KEY_F(12);
	bool enable_solenoid_control = false;

	SolenoidState[] solenoid_states = [
		{ 'a', "FL L", Sol.FLIPPER_LEFT },
		{ 'o', "FL R", Sol.FLIPPER_RIGHT },
		{ 'e', "DTB0", Sol.DTB0 },
		{ 'u', "SS 0", Sol.SLINGSHOT0 },
		{ 'i', "SS 1", Sol.SLINGSHOT1 },
		{ 'd', "BMP0", Sol.BUMPER0 },
		{ 'h', "BMP1", Sol.BUMPER1 },
		{ 't', "BMP2", Sol.BUMPER2 },
		{ 'n', "BLRT", Sol.BALL_RETURN },
		{ 's', "AUX0", Sol.AUX_0 },
		{ '-', "AUX1", Sol.AUX_1 }
	];

	Key[] key_table = [
		{ '1', '!', "FL_L ", Sw.FLIPPER_LEFT,  false },
		{ '2', '@', "FL_R ", Sw.FLIPPER_RIGHT, false },
		{ '3', '#', "DTB00", Sw.DTB0_0,        false },
		{ '4', '$', "DTB01", Sw.DTB0_1,        false },
		{ '5', '%', "DTB02", Sw.DTB0_2,        false },
		{ '6', '^', "DTB03", Sw.DTB0_3,        false },
		{ '7', '&', "DTB04", Sw.DTB0_4,        false },
		{ '8', '*', "SS_0 ", Sw.SLINGSHOT0,    false },
		{ '9', '(', "SS_1 ", Sw.SLINGSHOT1,    false },
		{ '0', ')', "BMP_0", Sw.BUMPER0,       false },
		{ '[', '{', "BMP_1", Sw.BUMPER1,       false },
		{ ']', '}', "BMP_2", Sw.BUMPER2,       false },

		{ '\'', '"', "BLO ", Sw.BALL_OUT,      false },
		{ ',',  '<', "HOL0", Sw.HOLE0,         false },
	];

	// Switch states

	this(Solenoids_ solenoids_, Switches_ switches_) {
		solenoids = new Solenoids(solenoids_, No.mask_get, No.mute_set);
		switches = new Switches(switches_, Yes.mask_get, No.mute_set);
		canvas = new Canvas(16, 40);
		logger = new Logger();

		nc.initscr();
		nc.cbreak();
		nc.nonl();
		nc.nodelay(nc.stdscr, true);
		nc.keypad(nc.stdscr, true);
		nc.curs_set(0);
		nc.noecho();

		nc.start_color();
		nc.use_default_colors();


		foreach(i; 0 .. colors_fg.length) {
			color_idx ~= nc.init_pair(cast(short)(i + 1), colors_fg[i], colors_bg[i]);
		}

		nc.bkgdset(' ' | COLOR_BLACK);

		nc.wclear(nc.stdscr);
		nc.wnoutrefresh(nc.stdscr);
		nc.doupdate();
	}

	~this() {
		nc.nocbreak();
		nc.noraw();
		nc.curs_set(1);
		nc.echo();
		nc.nl();
		endwin();
	}

	override void frame_start(Duration dt) {
		paint_canvas(Coord(2, 2));
		paint_switch_states(Coord(22, 2));
		paint_solenoid_states(Coord(25, 2));
		paint_log(Coord(28, 2));
		nc.wnoutrefresh(nc.stdscr);
		nc.doupdate();
		read_keys;
	}

	Solenoids solenoids;
	Switches switches;
	Canvas canvas;
	Logger logger;

	private:

		enum string color_symbols = "    ####";
		enum short[] colors_fg = [ nc.COLOR_WHITE, nc.COLOR_RED, nc.COLOR_GREEN, nc.COLOR_YELLOW, nc.COLOR_RED,    nc.COLOR_RED,   nc.COLOR_GREEN, nc.COLOR_YELLOW];
		enum short[] colors_bg = [ -1, nc.COLOR_RED, nc.COLOR_GREEN, nc.COLOR_YELLOW, nc.COLOR_YELLOW, nc.COLOR_BLACK, nc.COLOR_BLACK, nc.COLOR_RED   ];
		int[] color_idx;

		void paint_canvas(Coord pos) {
			foreach(int row; 0 .. canvas.size.row) {
				nc.move(pos.row + row, pos.column);
				foreach(int column; 0 .. canvas.size.column) {
					int c = canvas[row, column];
					nc.addch(color_symbols[c] | COLOR_PAIR(c + 1) | A_NORMAL);
					nc.addch(color_symbols[c] | COLOR_PAIR(c + 1) | A_NORMAL);
				}
			}
		}

		void paint_switch_states(Coord pos) {
			move(pos.row, pos.column);
			int i = 0;
			foreach(ref Key key; key_table) {
				attrset(switches[key.index] ? (COLOR_PAIR(0) | A_REVERSE) : COLOR_PAIR(0));
				printw(" %c%c:%s ", cast(char)key.toggle, cast(char)key.flip, key.str.toStringz);
				attrset(COLOR_PAIR(0));
				printw(" ");
				if(++i == 12) {
					pos.row++;
					move(pos.row, pos.column);
					i = 0;
				}
			}
			printw("\n\n   ");
		}

		void paint_solenoid_states(Coord pos) {
			nc.wmove(nc.stdscr, pos.row, pos.column);
			attrset(enable_solenoid_control ? (COLOR_PAIR(7) | A_REVERSE) : COLOR_PAIR(0));
			printw(" F1:ENABLE ");
			attrset(COLOR_PAIR(0));
			printw(" ");
			foreach(SolenoidState s; solenoid_states) {
				attrset(solenoids[s.index] ? (COLOR_PAIR(7) | A_REVERSE) : COLOR_PAIR(0));
				printw(" %c:%s ", s.key, s.str.toStringz);
				attrset(COLOR_PAIR(0));
				printw(" ");
			}
			attrset(COLOR_PAIR(0));
		}

		void paint_log(Coord pos) {
			uint i = 0;
			foreach(string line; logger) {
				nc.wmove(nc.stdscr, pos.row + i, pos.column);
				printw("%s", line.toStringz);

				// Print some spaces to erase whatever was before in this line
				for(int j=0; j<80 - line.length; j++) {
					printw(" ");
				}
				i++;
			}
		}

		void read_keys() {
			// reset all key states
			foreach(ref Key key; key_table) {
				switches.mask[key.index] = key.state;
			}

			// read & process key
			int ch = wgetch(stdscr);

			if(ch == enable_solenoid_control_key) {
				enable_solenoid_control = !enable_solenoid_control;

				// If user controls solenoids, cut off game logic
				solenoids.mute_set = enable_solenoid_control;
			}
			else if(ch == quit_key) {
				this.quit();
			}

			// Is this a switch-emulating key?
			foreach(ref Key key; key_table) {
				if(ch == key.toggle) {
					switches.mask[key.index] = !key.state;
					break;
				}
				else if(ch == key.flip) {
					key.state = !key.state;
					switches.mask[key.index] = key.state;
					break;
				}
			}

			// Is this a solenoid-controlling key?
			if(enable_solenoid_control) {
				foreach(ref SolenoidState solenoid; solenoid_states) {
					solenoids.decorated[solenoid.index] = (ch == solenoid.key);
				} // for solenoid
			} // if enable solenoid contral
		}

		alias canvas this;
}

