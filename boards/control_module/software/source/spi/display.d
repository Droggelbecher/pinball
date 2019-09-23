
import std.datetime;
import std.experimental.logger: tracef;
import core.thread;

import task;
import coordinate;
import buffer_canvas;
import canvas;

class Display(Spi): Task {
	enum Color {
		BLACK = 0,
		RED = 1,
		GREEN = 2,
		YELLOW = 3,
		ORANGE = 4,
		DARK_RED = 5,
		DARK_GREEN = 6,
		BLOOD_ORANGE = 7,

		COLORS = 8,
	}

	/+
	this(Spi spi, ubyte modules, Coord module_size) {
		this.modules = modules;
		this.module_size = module_size;
		this.module_buffer = new BufferCanvas(module_size);
		super(Coord(module_size.row, module_size.column * modules));
		//this.buffer = new ubyte[modules * module_size.area];
	}
	+/
	this(Spi spi, Coord module_size) {
		this.spi = spi;
		this.module_buffer = new BufferCanvas(module_size);
	}

	/+
	this(Spi spi, BufferCanvas buffer, Coord module_size) {
		this.spi = spi;
		this.modules = buffer.size.column / module_size.column;
		this.buffer = buffer;
		this.module_buffer = new BufferCanvas(module_size);
	}
	+/
	
	@property
	void source_buffer(BufferCanvas buffer) {
		this.buffer = buffer;
		this.modules = cast(ubyte)(buffer.size.column / module_buffer.size.column);
	}

	@property
	BufferCanvas source_buffer() {
		return this.buffer;
	}

	override
	void frame_start(Duration dt) {
		if(!buffer) {
			tracef("SPI Display: No source buffer!");
			return;
		}

		tracef("SPI Display: SS enable");
		spi.select_only(Spi.SlaveIndex.Display);

		for(int i = 0; i < modules; i++) {
			blit(
				buffer, Coord(0, i * module_buffer.size.column), module_buffer.size,
				module_buffer, Coord(0, 0)
			);
			// TODO: chances are the blit above already took some time,
			// can we remove this or only wait the remaining part of the 10 usecs?
			Thread.sleep(10.usecs);
			spi.send(module_buffer.buffer);
		}

		spi.deselect_all();
		tracef("SPI Display: SS disable");
	}

	private:
		Spi spi;
		ubyte modules = 0;
		//const Coord module_size;
		BufferCanvas buffer;
		BufferCanvas module_buffer;
		//ubyte[] screen;

}


