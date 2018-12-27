
import task;

class CursesInterface : Task {

	import nice.curses;
	import buffer_canvas;
	import coordinate;
	import std.stdio;
	import std.datetime;

	this() {
		canvas = new BufferCanvas(16, 40);

		Curses.Config cfg = {
			useColors: true,
			disableEcho: true,
			cursLevel: 0,
			nodelay: true,
			mode: Curses.Mode.cbreak,
			initKeypad: true
		};
		curses = new Curses(cfg);
		auto scr = curses.stdscr;

		foreach(i; 0 .. colors_fg.length) {
			color_idx ~= curses.colors.addPair(colors_fg[i], colors_bg[i]);
		}

		scr.clear;
		scr.refresh;
		curses.update;
	}

	override void frame_start(Duration dt) {
		paint_canvas();
	}

	BufferCanvas canvas;
	alias canvas this;

	private:

		string color_symbols = "    ####";
		StdColor[] colors_fg = [ StdColor.white, StdColor.red, StdColor.green, StdColor.yellow, StdColor.red,    StdColor.red,   StdColor.green, StdColor.yellow];
		StdColor[] colors_bg = [ StdColor.black, StdColor.red, StdColor.green, StdColor.yellow, StdColor.yellow, StdColor.black, StdColor.black, StdColor.red   ];
		short[] color_idx;

		void paint_canvas() {
			auto scr = curses.stdscr;
			foreach(int row; 0 .. canvas.size.row) {
				scr.move(row, 0);
				foreach(int column; 0 .. canvas.size.column) {
					int c = canvas[row, column];
					short cidx = color_idx[c];
					scr.addch(color_symbols[c], curses.colors[cidx]);
					scr.addch(color_symbols[c], curses.colors[cidx]);
				}
			}
			scr.refresh;
			curses.update;
		}

		Curses curses;
}

