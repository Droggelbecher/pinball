
import std.datetime: Duration, seconds, msecs;

import task: Task;
import five_eight:   font_5x8_size, font_5x8_data;
import font:         Font, StringCanvas;
import scrolling;
import coordinate;
import canvas;
import switchable;

class TextDisplay(alias iface): Task {

	alias blit = canvas.blit;
	alias blit = scrolling.blit;
	alias Font!(font_5x8_size) FontNormal;
	alias Interface = typeof(iface);

	mixin Switchable;

	StringCanvas!FontNormal text;
	FontNormal font_normal;

	Scrolling!(Interface.Canvas) scroll;

	this() {
		scroll = new Scrolling!(Interface.Canvas)(
			iface.canvas,
			Coord(160, iface.canvas.size.column)
		);
		font_normal = new FontNormal(font_5x8_data);
	}

	void s(string t, ubyte color = 1) {
		this.text = font_normal(t, color);
	}

	override
	void frame_start(Duration dt) {
		scroll.next_frame(dt);
		if(enabled) {
			blit(text, Coord(), text.size, scroll, Coord());
		}
	}
}
