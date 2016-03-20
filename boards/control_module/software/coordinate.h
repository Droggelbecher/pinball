
#ifndef COORDINATE_H
#define COORDINATE_H

/**
 *
 */
template<typename T = int>
class Coordinate {

	public:
		Coordinate()
			: row_(0), column_(0)
		{
		}

		Coordinate(int row, int column)
			: row_(row), column_(column)
		{
		}

		int row() const { return row_; }
		int column() const { return column_; }

		Coordinate& operator+=(const Coordinate& other) {
			row_ += other.row_;
			column_ += other.column_;
			return *this;
		}

		Coordinate& operator%=(const Coordinate& other) {
			row_ %= other.row_;
			column_ %= other.column_;
			return *this;
		}

	private:
		int row_;
		int column_;
};

#endif

