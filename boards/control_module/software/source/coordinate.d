
struct Coordinate(T = int) {
	this(T row, T column) {
		this.row = row;
		this.column = column;
	}

	T area() {
		return row * column;
	}

	T row;
	T column;
}

Coordinate!(T) make_coordinate(T)(T row, T column) {
	return Coordinate!(T)(row, column);
}

