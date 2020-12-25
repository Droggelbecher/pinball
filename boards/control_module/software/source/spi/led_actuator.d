
import std.datetime;
import std.random;

import task;

extern(C) {
	@nogc ubyte checksum(ubyte*, ubyte);
}

class LEDActuator(Spi, int SlaveIdx): Task {
	
	enum DATA_BYTES = 15;

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

	enum Lamp {
		DS_WEAPON,
		DS_LIGHT ,
		TARGET   ,
		VADER    ,
		FALCON   ,
		BMP0     ,
		BMP1     ,
		BMP2     ,
		SS0      ,
		SS1
	};


	this(Spi spi) {
		command[0] = cast(ubyte)(ID_INCREMENT * uniform(1, 8)); // have some non-zero start fo ID
		this.spi = spi;
	}

	LEDActuator mode(ColorMode color, AnimationMode animation) {
		command[0] = cast(ubyte)(
			  (((command[0] & ID_MASK) + ID_INCREMENT) & ID_MASK)
			| color | animation
		);
		return this;
	}

	LEDActuator mod(ubyte mod) {
		command[1] = mod;
		return this;
	}

	LEDActuator color0(ubyte[3] color) {
		command[2 .. 5] = color;
		return this;
	}
	//void color0(ubyte r, ubyte g, ubyte b) { color0([r, g, b]); }

	LEDActuator color1(ubyte[3] color) {
		command[5 .. 8] = color;
		return this;
	}
	//void color1(ubyte r, ubyte g, ubyte b) { color1([r, g, b]); }

	LEDActuator color(ubyte[3] color) {
		color0(color);
		color1(color);
		return this;
	}

	LEDActuator dir(ubyte dir) {
		command[8] = dir;
		return this;
	}

	LEDActuator dt(ubyte dt) {
		command[9] = dt;
		return this;
	}

	LEDActuator count(ubyte count) {
		command[10] = count;
		return this;
	}

	// convenience methods
	LEDActuator full(ubyte[3] color) {
		mode(ColorMode.GRADIENT, AnimationMode.FADEOUT);
		this.color(color);
		dt(0);
		mod(1);
		return this;
	}

	LEDActuator rotmod(ubyte[3] color, ubyte k, ubyte dt) {
		mode(ColorMode.MOD, AnimationMode.ROTATE);
		mod(k);
		color0(color);
		color1([0,0,0]);
		this.dt(dt);
		dir(0);
		return this;
	}

	LEDActuator lamp(Lamp index, bool on=true) {
		this[index] = on;
		return this;
	}

	bool opIndex(Lamp index) {
		int bit = index % 8;
		int byte_ = index / 8;
		return (command[11 + byte_] & (1 << cast(int)index)) != 0;
	}

	bool opIndexAssign(bool v, Lamp index) {
		int bit = index % 8;
		int byte_ = index / 8;
		if(v) {
			command[11 + byte_] |= (1 << cast(int)index);
		}
		else {
			command[11 + byte_] &= ~(1 << cast(int)index);
		}
		return this[index];
	}

	override void frame_start(Duration dt) {
		//command[DATA_BYTES] = checksum(command.ptr, cast(ubyte)DATA_BYTES);
		spi.transfer_and_check(cast(Spi.SlaveIndex)SlaveIdx, command);
	}

	private {
		Spi spi;
		ubyte[DATA_BYTES] command;
	}
}

alias LEDStripe(Spi) = LEDActuator!(Spi, Spi.SlaveIndex.LEDStripe);

