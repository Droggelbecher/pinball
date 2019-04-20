
import std.datetime;
import std.file: dirEntries, SpanMode;
import std.algorithm.sorting: sort;
import std.algorithm: map;
import std.array;
import std.typecons;

import task;
import interval: Interval;
import buffer_canvas;
import canvas;
import coordinate;

class Sprite(Target): Task {

	this(BufferCanvas[] frames, Duration dt, Target target, Flag!"loop" loop = No.loop) {
		this.fps_tracker = Interval!Duration(dt);
		this.frames = frames;
		this.target = target;
		this.loop = loop;
		this.i = 0;
	}

	override void frame_start(Duration dt) {
		if(i >= frames.length && loop) {
			i = 0;
		}

		if(i < frames.length) {
			blit(frames[i], Coord(0, 0), frames[0].size,
					target, Coord(0, 0));

			if(fps_tracker(dt)) {
				i++;
			}
		}
	}

	private:
		Interval!Duration fps_tracker;
		BufferCanvas[] frames;
		Target target;
		Flag!"loop" loop;
		int i;
}

class PNGSprite(Target): Sprite!Target {
	this(string path, Duration dt, Target target, Flag!"loop" loop = No.loop) {
		auto pngs = dirEntries(path, "*.png", SpanMode.shallow).array.sort;

		import read_png;
		super(pngs.map!read_png.array, dt, target, loop);
	}
}


