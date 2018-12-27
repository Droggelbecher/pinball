
import coordinate;
import std.traits;
import canvas;

class Font(Coordinate!() size_, int padding_ = 1) {
	static enum padding = padding_;
	static enum size = size_;

	this(ubyte[size_.area()][char] font_data) {
		this.font_data = font_data;
	}

	auto opIndex(char c) {
		return CharacterCanvas!Font(this.font_data[c]);
	}

	auto opCall(string s) {
		ubyte[][] bufs;
		foreach(int i, char c; s) {
			bufs ~= this.font_data[c];
		}
		return StringCanvas!Font(bufs);
	}

	ubyte[size_.area()][char] font_data;
}

struct CharacterCanvas(Font_) {
	alias Font_ Font;
	static immutable storage_type = StorageType.ColumnFirst;

	this(ubyte[] char_data) {
		this.buffer = char_data;
	}

	Coordinate!() size() { return Font.size; }

	ubyte[] buffer;
}

struct StringCanvas(Font_) {
	alias Font_ Font;
	static enum storage_type = StorageType.Other;
	static enum padding = Font.padding;

	this(ubyte[][] buffers) {
		this.buffers = buffers;
	}

	Coordinate!() character_size() { return Font.size; }
	Coordinate!() size() {
		Coordinate!() r = Coordinate!()(
			Font.size.row,
			cast(int)((Font.size.column + Font.padding) * buffers.length)
		);
		return r;
	}

	ubyte[][] buffers;
}


void blit(FromCanvas, ToCanvas)(
		FromCanvas from, Coordinate!() from_start, Coordinate!() size,
		ToCanvas to_, Coordinate!() to_start
)
	if(isInstanceOf!(StringCanvas, FromCanvas))
in {
	assert(size.nonnegative);
	assert(from_start in from.size);
	assert((from_start + size - Coord(1, 1)) in from.size);
	assert(to_start in to_.size);
	assert((to_start + size - Coord(1, 1)) in to_.size);
}
body {
	import canvas: blit;
	alias FromCanvas.Font Fnt;

	const int w = from.character_size.column + from.padding;
	int i_start = from_start.column / w;
	int delta = from_start.column % w;

	auto sz = Coord(
		min(size.row, from.character_size.row * w * from.buffers.length),
		min(size.column, from.character_size.column)
	);

	auto p = to_start;
	foreach(int i, ubyte[] buffer; from.buffers) {
		if(i < i_start) { continue; }
		blit(
			CharacterCanvas!(Fnt)(buffer),
			Coord(from_start.row, (i == i_start) ? delta : 0),
			sz, to_, p
		);
		p += Coord(0, w);
	}
}

