
import buffer_canvas: BufferCanvas;
import coordinate: Coord;
import std.string: splitLines;
import sprite: Sprite;
import std.datetime: Duration, msecs;
import std.typecons: Flag, Yes, No;
import display: Color;

BufferCanvas string_to_canvas(Coord size, ubyte[char] color_map, string s) {
	auto c = new BufferCanvas(size);
	foreach(row, line; s.splitLines) {
		if(row >= size.row) break;
		foreach(col, ch; line) {
			if(col >= size.column) break;
			size_t pos = (row * size.column + col);
			c.buffer[pos] = color_map[ch];
		}
	}
	return c;
}

BufferCanvas[] strings_to_frames(Coord size, ubyte[char] color_map, string[] strings) {
	BufferCanvas[] frames;
	foreach(string s; strings) {
		frames ~= string_to_canvas(size, color_map, s);
	}
	return frames;
}

