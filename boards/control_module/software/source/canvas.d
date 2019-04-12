
import std.experimental.logger;
import std.algorithm: min, max;
import coordinate;

enum StorageType {
	RowFirst,
	ColumnFirst,
	Other
}

@nogc
void blit(FromCanvas, ToCanvas)(
		FromCanvas from, Coordinate!() from_start, Coordinate!() size,
		ToCanvas to_, Coordinate!() to_start
)
if(FromCanvas.storage_type == StorageType.ColumnFirst
		&& ToCanvas.storage_type == StorageType.ColumnFirst)
in {
	assert(size.nonnegative);
	assert((from_start in from.size) || (from.size.area == 0));
	assert(((from_start + size - Coord(1, 1)) in from.size) || (from.size.area == 0));
	assert(to_start in to_.size);
	assert((to_start + size - Coord(1, 1)) in to_.size);
}
do {
	import core.stdc.string: memcpy;

	if(from.size.area == 0) {
		return;
	}

	int n_from = from.size.column;
	int n_to = to_.size.column;

	const ubyte *buf_from = from.buffer.ptr + from_start.row * n_from + from_start.column;
	ubyte *buf_to         = to_.buffer.ptr + to_start.row * n_to + to_start.column;

	for(int i = 0; i < size.row; i++) {
		memcpy(
				buf_to + i * n_to,
				buf_from + i * n_from,
				size.column
		);
	}
}

@nogc
void clear(ToCanvas)(ToCanvas to_) if(__traits(hasMember, ToCanvas, "buffer")) {
	to_.buffer[] = 0;
}

@nogc
void set_color(ToCanvas)(ToCanvas to_, ubyte c) if(__traits(hasMember, ToCanvas, "buffer")) {
	to_.buffer[] = c;
}


unittest {
	Coord from;
	Coord size;
	Coord to;
	int called = 0;

	void f(Coord from_, Coord size_, Coord to_) {
		from = from_;
		size = size_;
		to = to_;
		++called;
	}

	called = 0;
	crop!f(
			Coord(0, 0), Coord(8, 5), // source: complete canvas of size 8x5
			Coord(8, 5),
			Coord(4, 3), Coord(10, 30) // target: canvas of size 10x30 at (4, 3)
	);
	assert(called == 1);
	assert(from == Coord(0, 0));
	assert(to == Coord(4, 3));
	assert(size == Coord(6, 5));

	called = 0;
	crop!f(
			Coord(3, 0), Coord(8, 5), // skip first 3 rows, source area overlaps canvas by those
			Coord(8, 5),
			Coord(4, 3), Coord(10, 30) // target: canvas of size 10x30 at (4, 3)
	);
	assert(called == 1);
	assert(from == Coord(3, 0));
	assert(to == Coord(4, 3));
	assert(size == Coord(5, 5));
}

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
@nogc
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

@nogc
void blit_center(alias blit, FromCanvas, ToCanvas)(FromCanvas from, ToCanvas to_) {
	blit(
		from, Coord(), from.size, to_,
		(to_.size - from.size) / 2
	);
}


