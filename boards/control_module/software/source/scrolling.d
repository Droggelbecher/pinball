
import coordinate;
import canvas: blit;
import font: blit;

class Scrolling(Decorated) {

	import canvas: StorageType;
	enum storage_type = StorageType.Other;

	this(Decorated decorated, Coord virtual_size, Coordinate!double speed) {
		this.decorated = decorated;
		this.virtual_size = virtual_size;
		this.speed = speed;
		this.offset = Coordinate!double(0, 0);
	}

	@nogc
	Coord size() const { return decorated.size; }

	@nogc
	void next_frame(double dt) {
		static if(__traits(hasMember, Decorated, "next_frame")) {
			decorated.next_frame(dt);
		}
		offset += speed * dt;
		offset %= virtual_size;
	}

	Decorated decorated;
	Coord virtual_size;
	Coordinate!double speed;
	Coordinate!double offset;

}

void blit(SomeCanvas, Decorated)(
		SomeCanvas from, Coord from_start, Coord size,
		Scrolling!(Decorated) to_, Coord to_start
) {
	scroll(from, from_start, size, to_, to_start);
}


private:

/**
  Blit from source to target canvas cropping at canvas ends.

  @param f: Function (from, size, to) for blitting a rect of size $size at $from in source
			to a target coordniate $to. f is assumed to be able to handle only blits that are
			completely inside target canvases respectively.

  @param from_start: start position in source to blit from
  @param size: size of the rect to blit
  @param to_start: start position in target canvas
  @param to_size: size of target canvas

  Calls $f at most once.

  **/
void crop(alias f)(Coord from_start, Coord from_size, Coord size, Coord to_start, Coord to_size)
in {
	assert(to_start.lower(100000));
}
do {
	if(!size.nonnegative) { return; }
	if(!from_size.nonnegative) { return; }
	if(!to_size.nonnegative) { return; }
	if(!size.area || !(size - Coord(1, 1)).area) { return; }
	if(!from_size.area || !(from_size - Coord(1, 1)).area) { return; }
	if(!to_size.area || !(to_size - Coord(1, 1)).area) { return; }

	Coord to_end = to_start + size - Coord(1, 1);
	Coord from_end = from_start + size - Coord(1, 1);

	if(to_start.row < 0 || from_start.row < 0) {
		Coord cut = Coord(-min(from_start.row, to_start.row), 0);
		crop!f(from_start + cut, from_size, size - cut, to_start + cut, to_size);
		return;
	}
	if(to_end.row >= to_size.row || from_end.row >= from_size.row) {
		Coord cut = Coord(
				max(to_end.row - to_size.row + 1, from_end.row - from_size.row + 1),
				0
		);
		crop!f(from_start, from_size, size - cut, to_start, to_size);
		return;
	}
	if(to_start.column < 0 || from_start.column < 0) {
		Coord cut = Coord(0, -min(from_start.column, to_start.column));
		crop!f(from_start + cut, from_size, size - cut, to_start + cut, to_size);
		return;
	}
	if(to_end.column >= to_size.column || from_end.column >= from_size.column) {
		Coord cut = Coord(
				0,
				max(to_end.column - to_size.column + 1, from_end.column - from_size.column + 1),
		);
		crop!f(from_start, from_size, size - cut, to_start, to_size);
		return;
	}

	assert(from_start.nonnegative);
	assert(size.nonnegative);
	assert((to_start + size - Coord(1, 1)) in to_size);
	f(from_start, size, to_start);
}

/**
  Blit a source canvas (here called "virtual canvas") to a target canvas
  in a wrapping manner. I.e. where the blit goes out of the target canvas eg.
  on the right, that part is blit to the beginning (left) of the canvas.

  By moving the virtual canvas the well-known "marquee" effect is achieved.
**/
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

	// The part overlapping the virtual canvas on the bottom
	crop!f(
		virtual_start, virtual_size,
		size,
		target_start + virtual_start - Coord(virtual_size.row, 0), target_size
	);

	// TODO: for backwards scrolling also add overlapping on the left & top
}

void scroll(SourceCanvas, ScrollingCanvas)(
		SourceCanvas from,
		Coord from_start,
		Coord size,
		ScrollingCanvas scrolling,
		Coord scrolling_start
) {
	import font: blit;

	// 1. Crop from source onto virtual canvas
	crop!(

		// 2. Wrap virtual canvas onto target canvas
		(Coord from_start, Coord size, Coord to_start) =>
		wrap!(

			// 3. Do the actual blitting
			(Coord from_start, Coord size, Coord to_start) =>
			blit(from, from_start, size, scrolling.decorated, to_start)

		)(
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



