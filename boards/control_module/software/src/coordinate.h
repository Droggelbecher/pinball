
#ifndef COORDINATE_H
#define COORDINATE_H

#include <cmath>
#include <iostream>

namespace pinball {

/**
 *
 */
template<typename T = int>
class Coordinate {

	public:
		typedef T Ordinate;

		Coordinate()
			: row_(0), column_(0) {
		}

		Coordinate(T row, T column)
			: row_(row), column_(column) {
		}

		Coordinate(const Coordinate& c)
			: row_(c.row_), column_(c.column_) {
		}

		template<typename C>
		Coordinate(const C& c)
			: row_(c.row()), column_(c.column()) {
		}


		T row() const { return row_; }
		T column() const { return column_; }

		T& row() { return row_; }
		T& column() { return column_; }

		template<typename C>
		Coordinate<decltype(T() + typename C::Ordinate())> operator+(const C& other) {
			return { row() + other.row(), column() + other.column() };
		}

		template<typename C>
		Coordinate<decltype(T() - typename C::Ordinate())> operator-(const C& other) {
			return { row() - other.row(), column() - other.column() };
		}

		template<typename C>
		Coordinate<decltype(T() * typename C::Ordinate())> operator*(const C& other) const {
			return { row() * other.row(), column() * other.column() };
		}

		template<typename S>
		Coordinate<decltype(T() * S())> operator*(const S& scalar) const {
			return { row() * scalar, column() * scalar };
		}

		Coordinate& operator+=(const Coordinate& other) {
			row_ += other.row_;
			column_ += other.column_;
			return *this;
		}

		template<typename C>
		Coordinate<decltype(T() % typename C::Ordinate())> operator%(const C& other) {
			 return { row() % other.row(), column() % other.column() };
		}

		template<typename C>
		Coordinate<double> operator%(const C& other) {
			 return { fmod(row(), other.row()), fmod(column(), other.column()) };
		}

		template<typename C>
		Coordinate<decltype(T() % typename C::Ordinate())>& operator%=(const C& other) {
			row_ %= other.row();
			column_ %= other.column();
			if(row_ < 0) { row_ += other.row(); }
			if(column_ < 0) { column_ += other.column(); }
			return *this;
		}

		template<typename C>
		Coordinate<double>& operator%=(const C& other) {
			row_ = fmod(row_, other.row());
			column_ = fmod(column_, other.column());
			if(row_ < 0) { row_ += other.row(); }
			if(column_ < 0) { column_ += other.column(); }
			return *this;
		}

		Ordinate area() const {
			return row_ * column_;
		}

		bool contains(const Coordinate& other) const {
			return (other.row_ >= 0) && (other.column_ >= 0)
				&& (other.row_ < row_) && (other.column_ < column_);
		}

	private:
		T row_;
		T column_;
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const Coordinate<T>& coord) {
	os << "<" << coord.row() << ", " << coord.column() << ">";
	return os;
}

} // ns pinball

#endif // COORDINATE_H
