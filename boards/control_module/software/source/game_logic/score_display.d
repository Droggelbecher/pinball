
import std.datetime: Duration, seconds, msecs;
import std.experimental.logger;
import std.conv;
import core.stdc.stdio: snprintf;

import task: Task;
import canvas;
import scrolling;

import five_eight:   font_5x8_size, font_5x8_data;
import font:         Font, StringCanvas;

class ScoreDisplay(alias iface): Task {
	alias blit = canvas.blit;
	alias blit = scrolling.blit;
	alias Font!(font_5x8_size) FontNormal;
	alias Interface = typeof(iface);

	size_t score;
	size_t display_score;
	Duration show_score;
	StringCanvas!FontNormal score_text;
	FontNormal font_normal;

	this() {
		this.show_score = 0.msecs;
		this.score = 0;
		this.font_normal = new FontNormal(font_5x8_data);
	}

	override
	void frame_start(Duration dt) {
		this.show_score -= dt;
		if(display_score < score) {
			display_score += cast(int)(
				100.0 * (score - display_score + 10) / (this.show_score.total!"msecs" + 10.0)
			);
			if(display_score > score) {
				display_score = score;
			}
			render_score();
		}
		if(show_score > 0.msecs) {
			iface.canvas.clear;
			blit_center!(canvas.blit)(score_text, iface.canvas);
		}
	}

	void add_score(int score) {
		this.score += score;
		iface.logger.logf("Score: %d", this.score);
		this.show_score = 2000.msecs;
		render_score();
	}

	void render_score() {
		char[10] score_string;
		snprintf(score_string.ptr, score_string.length, "%d", this.display_score);
		// Alas, not entirely nogc, but lets try to reduce the per-frame allocations to a necessary minimum
		this.score_text = font_normal(to!string(score_string.ptr));
	}
}

