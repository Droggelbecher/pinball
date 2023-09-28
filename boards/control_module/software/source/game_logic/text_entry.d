
import coordinate;
import std.datetime: Duration, seconds, msecs;
import task;
import switchable;
import std.range: enumerate;
import canvas:       blit, blit_center, clear, set_color;
import font;
import display: Color;

class TextEntry(Canvas_, Input_, Font_): Task {
	mixin Switchable switchable;

	alias Canvas = Canvas_;
	alias Input = Input_;
	alias Command = Input.Command;
	alias Font = Font_;

	Font _font;
	Canvas canvas;
	Input input;

	this(Canvas canvas, Input input, Font font, int n) {
		this._value = new char[n];
		this._value[0..$] = 'A';
		this.daemon = false;
		this.canvas = canvas;
		this.input = input;
		this._font = font;
		this.update_render;
	}

	void reset() {
		this._value[0..$] = 'A';
		this.active_index = 0;
		this.update_render;
	}

	override
	void frame_start(Duration dt) {
		if(!enabled) {
			return;
		}

		Coord p;
		foreach(c; rendered) {
			blit(c, Coord(), c.size, this.canvas, p);
			p.column += c.size.column;
		}
	}

	/**
	  Interact with user to change `value`, return when done.
	  */
	override
	void run() {
		input.on;

		while(true) {
			yield(() => input.has_command);
			//yield(100.msecs);
			
			Command c = input.pop_command;
			if(c == Command.NEXT) {
				_value[active_index]++;
				if(_value[active_index] > 'Z') {
					_value[active_index] = 'A';
				}
				update_render;
			}
			else if(c == Command.SELECT) {
				active_index++;
				update_render;
				if(active_index >= _value.length) {
					input.off;
					switchable.off;
					return;
				}
			}
		}
	}

	string value() {
		return this._value.dup;
	}

	void on(bool enabled=true) {
		input.on(enabled);
		switchable.on(enabled);
	}

	private {

		void update_render() {
			rendered.length = _value.length;
			foreach(i, c; _value.enumerate()) {
				if(i == this.active_index) {
					rendered[i] = this._font(c, active_color);
				}
				else {
					rendered[i] = this._font(c, inactive_color);
				}
			}
		}

		ubyte active_color = Color.GREEN;
		ubyte inactive_color = Color.YELLOW;
		char[] _value;
		int active_index = 0;
		StringCanvas!Font[] rendered;
	}

}
