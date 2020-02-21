
import std.datetime: Duration, seconds, msecs;
import std.experimental.logger;
import std.conv;
import core.stdc.stdio: snprintf;

import task: Task;
import canvas;
import scrolling;

import five_eight:   font_5x8_size, font_5x8_data;
import font:         Font, StringCanvas;
import player: Player;
import switchable;

class ScoreDisplay(alias iface): Task {
	alias blit = canvas.blit;
	alias blit = scrolling.blit;
	alias Font!(font_5x8_size) FontNormal;
	alias Interface = typeof(iface);

	size_t display_score;
	Duration show_score;
	StringCanvas!FontNormal score_text;
	FontNormal font_normal;

	mixin Switchable;

	private {
		Player _player;
	}

	this() {
		this.show_score = 0.msecs;
		this.font_normal = new FontNormal(font_5x8_data);
	}

	override
	void frame_start(Duration dt) {
		if(_player is null) {
			return;
		}

		this.show_score -= dt;
		if(display_score < _player.score) {
			display_score += cast(int)(
				100.0 * (_player.score - display_score + 10) / (this.show_score.total!"msecs" + 10.0)
			);
			if(display_score > _player.score) {
				display_score = _player.score;
			}
			render_score();
		}

		if(enabled) {
		//if(show_score > 0.msecs) {
			iface.canvas.clear;
			blit_center!(canvas.blit)(score_text, iface.canvas);
		//}
		}
	}

	@property
	Player player(Player value) {
		this._player = value;

		// set display score to a little less so we show tehe new players score
		this.display_score = cast(size_t)(this._player.score * 0.9);
		return this._player;
	}

	void add_score(int score) {
		this._player.score += score * this._player.multiplier;
		//iface.logger.logf("Score: %d", this._player.score);
		this.show_score = 2000.msecs;
		render_score();
	}

	void render_score() {
		char[20] score_string;
		snprintf(score_string.ptr, score_string.length, "     x%d\n%d", this._player.multiplier, this.display_score);
		// Alas, not entirely nogc, but lets try to reduce the per-frame allocations to a necessary minimum
		this.score_text = font_normal(to!string(score_string.ptr));
	}
}

