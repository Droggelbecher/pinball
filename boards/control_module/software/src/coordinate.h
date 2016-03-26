
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

		Coordinate operator+(const Coordinate& other) {
			Coordinate r(*this);
			r += other;
			return r;
		}

		Coordinate& operator+=(const Coordinate& other) {
			row_ += other.row_;
			column_ += other.column_;
			return *this;
		}

		Coordinate operator%(const Coordinate& other) {
			Coordinate r(*this);
			r %= other;
			return r;
		}

		Coordinate& operator%=(const Coordinate& other) {
			row_ %= other.row_;
			column_ %= other.column_;
			return *this;
		}

		bool contains(const Coordinate& other) const {
			return (other.row_ <= row_) && (other.column_ <= column_);
		}

	private:
		int row_;
		int column_;
};

#endif // COORDINATE_H
