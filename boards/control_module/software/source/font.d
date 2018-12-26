
import coordinate;

class Font(Coordinate!() size_) {

	import canvas;

	Coordinate!() size() { return size_; }

	ubyte[size_.area()][char] font_data;

	this(ubyte[size_.area()][char] font_data) {
		this.font_data = font_data;
	}

	struct CharacterCanvas {
		static immutable storage_type = StorageType.ColumnFirst;

		this(ubyte[] char_data) {
			this.buffer = char_data;
		}

		Coordinate!() size() { return size_; }

		ubyte[] buffer;
	}

	CharacterCanvas opIndex(char c) {
		return CharacterCanvas(this.font_data[c]);
	}
}



