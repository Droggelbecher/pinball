
import std.datetime;

class LEDActuator(Spi, int SlaveIdx) {

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

	void set_mod(ubyte mod) {
		command[1] = mod;
	}

	void set_color0(ubyte r, ubyte g, ubyte b) {
		command[2] = r;
		command[3] = g;
		command[4] = b;
	}

	void set_color1(ubyte r, ubyte g, ubyte b) {
		command[5] = r;
		command[6] = g;
		command[7] = b;
	}

	void set_dir(ubyte dir) {
		command[8] = dir;
	}

	void set_dt(ubyte dt) {
		command[9] = dt;
	}

	void set_count(ubyte count) {
		command[10] = count;
	}

	@nogc
	void frame_start(Duration dt) {
		spi.transfer_and_check(cast(Spi.SlaveIndex)SlaveIdx, command);
	}

	private {
		Spi spi;
		ubyte[11] command;
	}
}

alias LEDStripe(Spi) = LEDActuator!(Spi, Spi.SlaveIndex.LEDStripe);

