
import std.datetime: Duration, seconds, msecs;

import task: Task;
import five_eight:   font_5x8_size, font_5x8_data;
import font:         Font, StringCanvas;
import scrolling;

class TextDisplay(alias iface): Task {

	alias blit = canvas.blit;
	alias blit = scrolling.blit;
	alias Font!(font_5x8_size) FontNormal;
	alias Interface = typeof(iface);

	StringCanvas!FontNormal text;
	bool enable;
	FontNormal font_normal;

	Scrolling!(Interface.Canvas) scroll;

	this() {
		scroll = new Scrolling!(Interface.Canvas)(
			iface.canvas,
			Coord(80, iface.canvas.size.column)
		);
		font_normal = new FontNormal(font_5x8_data);
		enable = false;
	}

	void s(string t, ubyte color = 1) {
		this.text = font_normal(t, color);
	}

	void off() {
		this.enable = false;
	}

	void on() {
		this.enable = true;
	}

	override
	void frame_start(Duration dt) {
		scroll.next_frame(dt);
		if(enable) {
			blit(text, Coord(), text.size, scroll, Coord());
		}
	}

	//void blank(Duration t = 100.msecs) {
		//enable = false;
		//yield(t);
	//}

	//void blink(Duration duration = 1000.msecs, Duration interval = 100.msecs) {
		//auto t = 0.msecs;
		//while(t < duration) {
			//enable = !enable;
			//yield(interval);
			//t += interval;
		//}
	//}
}
