
import coordinate;
import canvas;

class BufferCanvas {

	static immutable storage_type = StorageType.ColumnFirst;

	this(CArgs...)(CArgs cargs) {
		this(make_coordinate(cargs));
	}

	this(Coordinate!() size) {
		this.size = size;
		buffer = new ubyte[size.area];
	}

	ubyte opIndexAssign(CArgs...)(ubyte color, CArgs cargs) {
		auto c = make_coordinate(cargs);
		buffer[c.column + c.row * size.column] = color;
		return color;
	}

	ubyte opIndex(CArgs...)(CArgs cargs) {
		auto c = make_coordinate(cargs);
		return buffer[c.column + c.row * size.column];
	}

	Coordinate!() size;

	//private:
	ubyte[] buffer;
}

