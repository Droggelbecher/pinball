
import std.datetime;

import task;

class LEDActuator(Spi, int SlaveIdx): Task {

	enum ColorMode {
		MOD = 0, // light up every k'th LED in this color
		GRADIENT = 1, // gradient (color0 -> color1)

		BITS = 2,
		MASK = 0x03 //(1 << (COLOR_BITS + 1)) - 1
	};

	enum AnimationMode {
		ROTATE = 0x00,
		FADEOUT = 0x04,
		FLASH = 0x08,
		BITS = 3,
		MASK = 0x1c
	};

	enum ID_MASK = 0xe0;
	enum ID_INCREMENT = 0x20;

	this(Spi spi) {
		this.spi = spi;
	}

	void set_command(ColorMode color, AnimationMode animation) {
		command[0] = cast(ubyte)(
			  (((command[0] & ID_MASK) + ID_INCREMENT) & ID_MASK)
			| color | animation
		);
	}

	void set_mod(ubyte mod) { command[1] = mod; }

	void set_color0(ubyte[3] color) { command[2 .. 5] = color; }
	void set_color0(ubyte r, ubyte g, ubyte b) { set_color0([r, g, b]); }

	void set_color1(ubyte[3] color) { command[5 .. 8] = color; }
	void set_color1(ubyte r, ubyte g, ubyte b) { set_color1([r, g, b]); }

	void set_color(ubyte[3] color) {
		set_color0(color);
		set_color1(color);
	}

	void set_dir(ubyte dir) { command[8] = dir; }
	void set_dt(ubyte dt) { command[9] = dt; }
	void set_count(ubyte count) { command[10] = count; }

	// convenience methods
	void full(ubyte[3] color) {
		set_command(ColorMode.GRADIENT, AnimationMode.FADEOUT);
		set_color(color);
		set_dt(0);
	}

	void mod(ubyte[3] color, ubyte k, ubyte dt) {
		set_command(ColorMode.MOD, AnimationMode.ROTATE);
		set_mod(k);
		set_color(color);
		set_dt(dt);
		set_dir(0);
	}


	@nogc override
	void frame_start(Duration dt) {
		spi.transfer_and_check(cast(Spi.SlaveIndex)SlaveIdx, command);
	}

	private {
		Spi spi;
		ubyte[11] command;
	}
}

alias LEDStripe(Spi) = LEDActuator!(Spi, Spi.SlaveIndex.LEDStripe);

