
import coordinate;
import std.traits;
import canvas;
import std.stdio;

class Font(Coordinate!() size_, int padding_ = 0) {
	static enum padding = padding_;
	static enum size = size_;

	this(ubyte[size_.area()][char] font_data) {
		this.font_data = font_data;
	}

	auto opIndex(char c) {
		return CharacterCanvas!Font(this.font_data[c]);
	}

	auto opCall(string s) {
		// row x col x data
		ubyte[size_.area()][][] bufs = new ubyte[size_.area()][][](1, 1);
		int row = 0;
		int col = 0;
		int max_row_len = 0;

		foreach(char c; s) {
			if(c == '\n') {
				++row;
				max_row_len = max(max_row_len, col);
				col = 0;
				bufs ~= new ubyte[size_.area()][](1);
			}
			else {
				bufs[row].length = max(bufs[row].length, col + 1);
				bufs[row][col] = this.font_data[c];
				++col;
			}
		}
		auto r = StringCanvas!Font(bufs);
		r.max_row_len = max(max_row_len, col);
		return r;
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

	// [row][column][character-data]
	this(ubyte[Font.size.area()][][] buffers) {
		this.buffers = buffers;
	}

	Coord character_size() const { return Font.size; }
	Coord size() const {
		Coord r = Coord(
			cast(int)((Font.size.row + Font.padding) * buffers.length),
			cast(int)((Font.size.column + Font.padding) * max_row_len)
		);
		return r;
	}

	int max_row_len; // # characters
	ubyte[Font.size.area()][][] buffers;
}

@nogc
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
	import core.stdc.stdio;
	alias FromCanvas.Font Fnt;

	const int w = from.character_size.column + from.padding;
	const int h = from.character_size.row + from.padding;

	static struct ActualBlit {
		ubyte[Fnt.size.area()] buffer;
		ToCanvas to_;
		Coord size_;

		@nogc
		void opCall(Coord from_start_, Coord size_, Coord to_start_) {
			debug(font_blitting_position) {
				fprintf(stderr, "  -> from: %d/%d sz: %d/%d to: %d/%d\n",
						from_start_.row, from_start_.column,
						size_.row, size_.column,
						to_start_.row, to_start_.column);
				fflush(stderr);
			}

			this.size_ = size_;

			blit(
				CharacterCanvas!Fnt(buffer),
				from_start_, size_,
				to_, to_start_
			);
		}
	}

	Coord offs;
	ActualBlit actual_blit;
	actual_blit.to_ = to_;

	foreach(int n_row, ubyte[Fnt.size.area()][] row; from.buffers) {
		foreach(int i, ubyte[Fnt.size.area()] buffer; row) {
			actual_blit.buffer = buffer;

			auto start = from_start + offs - Coord(n_row * h, i * w);

			debug(font_blitting_position) {
				fprintf(stderr, "row: %d col: %d\n from: %d/%d start: %d/%d to: %d/%d offs: %d/%d size: %d/%d\n",
					n_row, i,
					from_start.row, from_start.column,
					start.row, start.column,
					to_start.row, to_start.column,
					offs.row, offs.column,
					size.row, size.column,
				);
				fflush(stderr);
			}

			crop!actual_blit(
				start, from.character_size,
				size - offs,
				to_start + offs, to_.size
			);

			offs += Coord(0, w);
		}
		offs.row += actual_blit.size_.row + from.padding;
		offs.column = 0;

	}
}
