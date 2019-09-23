
import std.datetime: Duration, seconds, msecs;
import std.experimental.logger;

import buffer_canvas;
import task;
import coordinate;
import canvas;

class TestableDisplay(Display): Task {

	this(Display display) {
		this.display = display;
	}

	@property
	void source_buffer(BufferCanvas buffer) {
		orig_source_buffer = buffer;
	}

	@property
	BufferCanvas source_buffer() {
		check_buffers();
		return display.source_buffer;
	}

	override
	void frame_start(Duration dt) {
		check_buffers();
		if(test) {
			update_test_buffer(dt);
		}
		display.frame_start(dt);
	}

	private {
		void check_buffers() {
			if(test) {
				if(!this.test_buffer) {
					this.test_buffer = new BufferCanvas(display.source_buffer.size);
				}
				display.source_buffer = this.test_buffer;
			}
			else {
				display.source_buffer = this.orig_source_buffer;
			}
		}

		void update_test_buffer(Duration dt) {
			static Duration cum;
			cum += dt;
			static Display.Color color = cast(Display.Color)0;
			if(cum >= 1000.msecs) {
				cum -= 1000.msecs;
				color++;
				if(color >= Display.Color.COLORS) {
					color = cast(Display.Color)0;
				}

				test_buffer.set_color(cast(ubyte)color);
				ubyte color2 = (cast(ubyte)color + 1) % 8;
				for(int mod = 0; mod < 5; mod++) {
					for(int w = 1; w <= mod + 4; w++) {
						for(int i = 0; i < w; i++) {
							test_buffer[w - 1, mod*8 + i] = color2;
							test_buffer[w - 1 + 8, mod*8 + i] = color2;
						}
					}
				}
			}
		} // update
	} // private

	bool test = false;
	Display display;

	private:
		BufferCanvas test_buffer;
		BufferCanvas orig_source_buffer;

}

