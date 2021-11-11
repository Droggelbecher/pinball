
import std.traits;
import std.stdio;
import std.algorithm: max;

import coordinate;
import canvas;

class Font(Coordinate!() size_, int padding_ = 0) {
	static enum padding = padding_;
	static enum size = size_;

	this(ubyte[size_.area()][char] font_data) {
		this.font_data = font_data;
		assert(this.font_data != null);
	}

	auto opCall(dchar c, ubyte color = 1) {
		return opCall(cast(string)[c], color);
	}

	auto opCall(string s, ubyte color = 1) {
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

				// TODO: This actually copies. Can we change this?
				// (then again this is a 5x8 bytes copy, a slice would already be 2x8 bytes
				//  + potential slowdown for indirection)
				bufs[row][col] = this.font_data[c];
				++col;
			}
		}
		auto r = StringCanvas!Font(bufs, max(max_row_len, col), color);
		return r;
	}

	ubyte[size_.area()][char] font_data;
}

struct StringCanvas(Font_) {
	alias Font_ Font;
	static enum storage_type = StorageType.ColumnFirst;
	static enum padding = Font.padding;

	this(ubyte[Font.size.area()][][] char_buffers, int max_row_len, ubyte color) {
		Coord per_char = Font.size + Coord(padding, padding);
		this.size = per_char * Coord(cast(int)char_buffers.length, max_row_len);
		this.buffer = new ubyte[this.size.area];

		// Row/column of current character (# characters)
		foreach(n_row, row; char_buffers) {
			foreach(n_col, buf; row) {

				// Row inside the Character buffer (# pixels)
				for(int c_row = 0; c_row < Font.size.row; c_row++) {
					size_t pos = (
						  n_col * per_char.column
						+ n_row * max_row_len * per_char.area
						+ c_row * max_row_len * per_char.column
					);

					this.buffer[pos .. (pos + Font.size.column)] =
						buf[c_row * Font.size.column .. (c_row + 1) * Font.size.column] * color;
				} // for c_row
			} // foreach n_col
		} // foreach n_row
	} // this()

	@nogc
	Coord character_size() const { return Font.size; }

	Coord size;
	ubyte[] buffer;
}

