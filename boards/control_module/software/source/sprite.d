
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

/**
  A canvas that changes its content periodically
  to be used for animations.
 */
class Sprite : Task {
	static enum storage_type = BufferCanvas.storage_type;

	this(BufferCanvas[] frames, Duration dt, Flag!"loop" loop = No.loop) {
		// TODO: Maybe assert same size?

		this.fps_tracker = Interval!Duration(dt);
		this.frames = frames;
		this.loop = loop;
		this.i = 0;
		this.buffer = frames[i].buffer;
		this.size = frames[i].size;
	}

	void rewind() {
		i = 0;
		fps_tracker.reset();
	}

	override void frame_start(Duration dt) {
		if(fps_tracker(dt)) {
			i++;
			if(i >= frames.length && loop) {
				i = 0;
			}
			if(i < frames.length) {
				buffer = frames[i].buffer;
				size = frames[i].size;
			}
		}
	}

	public:
		ubyte[] buffer;
		Coord size;
		Flag!"loop" loop;

	private:
		Interval!Duration fps_tracker;
		BufferCanvas[] frames;
		int i;
}

