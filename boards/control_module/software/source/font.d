
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

	this(const ubyte[] char_data) {
		this.buffer = char_data;
	}

	Coordinate!() size() const { return Font.size; }

	const ubyte[] buffer;
}

struct StringCanvas(Font_) {
	alias Font_ Font;
	static enum storage_type = StorageType.Other;
	static enum padding = Font.padding;

	this(ubyte[][] buffers) {
		this.buffers = buffers;
	}

	Coordinate!() character_size() const { return Font.size; }
	Coordinate!() size() const {
		Coordinate!() r = Coordinate!()(
			Font.size.row,
			cast(int)((Font.size.column + Font.padding) * buffers.length)
		);
		return r;
	}

	ubyte[][] buffers;
}


void blit(FromCanvas: StringCanvas!Font_, ToCanvas, Font_)(
		FromCanvas from, Coordinate!() from_start, Coordinate!() size,
		ToCanvas to_, Coordinate!() to_start
)
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
		min(size.row, from.character_size.row),
		min(size.column, from.character_size.column * w * from.buffers.length)
	);

	int i_end = ((from_start + sz).column) / w;

	auto p = to_start;
	foreach(int i, const(ubyte[]) buffer; from.buffers) {
		if(i >= i_start && i < i_end) {
			blit(
				CharacterCanvas!(Fnt)(buffer),
				Coord(from_start.row, (i == i_start) ? delta : 0),
				Coord(
					min(from.character_size.row, size.row),
					min(from.character_size.column, size.column, to_.size.column - p.column)
				),
				to_, p
			);
		}
		p += Coord(0, w);
	}
}

