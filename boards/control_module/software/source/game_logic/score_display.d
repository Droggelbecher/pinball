
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

/**
  Display a players score, multiplier and balls.
  */
class ScoreDisplay(alias iface): Task {
	alias blit = canvas.blit;
	alias blit = scrolling.blit;
	alias Font!(font_5x8_size) FontNormal;
	alias Interface = typeof(iface);
	alias DColor = Interface.Display.Color;

	enum Duration SCORE_COUNTUP_TIME = 2000.msecs;
	enum int MAX_DISPLAY_SCORE = 99999999;

	// Score currently displayed, might be different from player score for "counting up" effect
	size_t display_score;
	// How long yet to count up
	Duration show_score;
	StringCanvas!FontNormal score_text;
	FontNormal font_normal;
	DColor color;

	mixin Switchable;

	private {
		Player _player;
	}

	this(DColor color = DColor.YELLOW) {
		this.show_score = SCORE_COUNTUP_TIME;
		this.font_normal = new FontNormal(font_5x8_data);
		this.color = color;
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
			iface.canvas.clear;
			blit_center!(canvas.blit)(score_text, iface.canvas);
		}
	}

	// Show score of this player
	@property
	Player player(Player value) {
		this._player = value;
		this.display_score = 0;
		render_score();
		return this._player;
	}

	void add_score(int score) {
		this._player.score += score * this._player.multiplier;
		this.show_score = SCORE_COUNTUP_TIME;
		render_score();
	}

	void render_score() {
		char[20] score_string;
		auto s = this.display_score;
		if(s > MAX_DISPLAY_SCORE) {
			s = MAX_DISPLAY_SCORE;
		}
		snprintf(score_string.ptr, score_string.length, "%2d\x03  x%d\n%8ld", this._player.balls, this._player.multiplier, s);
		// Alas, not entirely nogc, but lets try to reduce the per-frame allocations to a necessary minimum
		this.score_text = font_normal(to!string(score_string.ptr), cast(ubyte)(this.color));
	}
}

