
import std.algorithm: min, max;
import std.traits;

struct Coordinate(T = int) {
	alias T Ordinate;
	alias TemplateOf!(Coordinate) CoordinateT;

	this(T row, T column) {
		this.row = row;
		this.column = column;
	}

	T area() const {
		return row * column;
	}

	Coordinate opOpAssign(string Op, Other)(Other other)
		if(isInstanceOf!(CoordinateT, Other))
	{
		mixin("this.row " ~ Op ~ "= other.row;");
		mixin("this.column " ~ Op ~ "= other.column;");
		return this;
	}

	auto opBinary(string Op, Other)(Other other) const
		if(isInstanceOf!(CoordinateT, Other) && Op != "in")
	{
		return Coordinate!(CommonType!(T, Other.Ordinate))(
			mixin("this.row " ~ Op ~ " other.row"),
			mixin("this.column " ~ Op ~ " other.column")
		);
	}

	auto opBinary(string Op, Other)(Other other) const
		if(isInstanceOf!(CoordinateT, Other) && Op == "in")
	{
		return row >= 0
			&& row < other.row
			&& column >= 0
			&& column < other.column;
	}

	auto opBinary(string Op, Other)(Other other) const
		if(isScalarType!Other)
	{
		return Coordinate!(CommonType!(T, Other))(
			mixin("this.row " ~ Op ~ " other"),
			mixin("this.column " ~ Op ~ " other")
		);
	}

	bool lower(Other)(Other other) const
		if(isScalarType!Other)
	{
		return (row < other) && (column < other);
	}

	bool greater(Other)(Other other) const
		if(isScalarType!Other)
	{
		return (row > other) && (column > other);
	}

	bool nonnegative() const
	{
		return (row >= 0) && (column >= 0);
	}

	auto opCast(Target)()
		if(isInstanceOf!(CoordinateT, Target))
	{
		return Target(
			cast(Target.Ordinate)row,
			cast(Target.Ordinate)column
		);
	}

	Coordinate crop(Coordinate other) const {
		return Coordinate(
			max(min(row, other.row), 0),
			max(min(column, other.column), 0)
		);
	}

	T row;
	T column;
}

alias Coordinate!(int) Coord;

Coordinate!(T) make_coordinate(T)(T row, T column) {
	return Coordinate!(T)(row, column);
}

