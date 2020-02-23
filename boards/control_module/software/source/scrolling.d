
import std.experimental.logger;

import coordinate;
import canvas;

class Scrolling(Decorated) {

	import std.datetime;
	import canvas: StorageType;
	enum storage_type = StorageType.Other;

	this(
			Decorated decorated,
			Coord virtual_size,
			Coordinate!double speed = Coordinate!double(0, 0)
	) {
		this.decorated = decorated;
		this.virtual_size = virtual_size;
		this.speed = speed;
		this.offset = Coordinate!double(0, 0);
	}

	@nogc void reset() { offset = Coordinate!double(0, 0); }
	@nogc void stop() { speed = Coordinate!double(0, 0); }
	@nogc Coord size() const { return decorated.size; }

	@nogc
	void next_frame(Duration dt) {
		static if(__traits(hasMember, Decorated, "next_frame")) {
			decorated.next_frame(dt);
		}
		offset += speed * dt.total!"usecs" / 1_000_000.0;
		offset %= virtual_size;
	}

	Decorated decorated;
	Coord virtual_size;
	Coordinate!double speed;
	Coordinate!double offset;

}

void blit(FromCanvas, Decorated)(
		FromCanvas from, Coord from_start, Coord size,
		Scrolling!(Decorated) to_, Coord to_start
) {
	//tracef("scroll blit");
	scroll(from, from_start, size, to_, to_start);
}


private:

/**
  Blit a source canvas (here called "virtual canvas") to a target canvas
  in a wrapping manner. I.e. where the blit goes out of the target canvas eg.
  on the right, that part is blit to the beginning (left) of the canvas.

  By moving the virtual canvas the well-known "marquee" effect is achieved.
**/
@nogc
void wrap(alias f)(
		Coord virtual_start,
		Coord size,
		Coord target_start,
		Coord virtual_size,
		Coord target_size
		)
in {
	assert(virtual_start.nonnegative);
	assert(target_start.greater(-100000));
}
do {

	// "Original" blit
	crop!f(
		virtual_start, virtual_size,
		size,
		target_start + virtual_start, target_size
	);

	// The part overlapping the virtual canvas to the right
	crop!f(
		virtual_start, virtual_size,
		size,
		target_start + virtual_start - Coord(0, virtual_size.column), target_size
	);

	// The part overlapping the virtual canvas to the left
	crop!f(
		virtual_start, virtual_size,
		size,
		target_start + virtual_start + Coord(0, virtual_size.column), target_size
	);

	// The part overlapping the virtual canvas on the bottom
	crop!f(
		virtual_start, virtual_size,
		size,
		target_start + virtual_start - Coord(virtual_size.row, 0), target_size
	);

	// The part overlapping the virtual canvas on the top
	crop!f(
		virtual_start, virtual_size,
		size,
		target_start + virtual_start + Coord(virtual_size.row, 0), target_size
	);
}

@nogc
void scroll(SourceCanvas, ScrollingCanvas)(
		SourceCanvas from,
		Coord from_start,
		Coord size,
		ScrollingCanvas scrolling,
		Coord scrolling_start
) {
	import canvas: blit;

	static struct ActualBlit {
		SourceCanvas from;
		Coord size;
		ScrollingCanvas scrolling;

		@nogc
		void opCall(Coord from_start, Coord size, Coord to_start) {
			blit(from, from_start, size, scrolling.decorated, to_start);
		}
	}
	ActualBlit actual_blit = { from, size, scrolling };

	// 1. Crop from source onto virtual canvas
	crop!(

		// 2. Wrap virtual canvas onto target canvas
		(Coord from_start, Coord size, Coord to_start) =>
		wrap!(actual_blit)(
			from_start, size, // start & size in virtual space
			// move target position by current scrolling offset
			cast(Coord)(to_start + scrolling.offset),
			scrolling.virtual_size, // wrap on virtual canvas size
			scrolling.decorated.size
		)
	)(
		from_start, from.size,
		size,
		scrolling_start, scrolling.virtual_size
	);
}



