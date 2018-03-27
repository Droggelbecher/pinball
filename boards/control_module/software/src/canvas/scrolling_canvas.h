
#ifndef SCROLLING_CANVAS_H
#define SCROLLING_CANVAS_H

#include <cassert>
#include <iostream>

#include "coordinate.h"

/**
 *
 */
template<typename TDecorated>
class ScrollingCanvas {
	public:
		ScrollingCanvas(TDecorated& decorated, Coordinate<double> speed)
			: decorated_(decorated), speed_(speed) { }

		void next_frame(double dt) {
			offset_ += speed_ * dt;
			offset_ %= size();
		}

		void set_pixel(Coordinate<> c, uint8_t color) {
			/*
			 *  +-----------------------------------+
			 *  | A|                         |  B   |
			 *  |  |                         |      |
			 *  |--+                         +------|
			 *  |                                   |
			 *  |                                   |
			 *  |                                   |
			 *  |                                   |
			 *  |                          offset_  |
			 *  |--+                         X------|-----+
			 *  | C|                         |  D   |     |
			 *  +-----------------------------------X     |
			 *           [R1]                |    size()  |
			 *                               |            |
			 *                               +------------+
			 *  +---------+                       [R2]
			 *  |   D  : C|
			 *  |......:..|
			 *  |   B  : A|
			 *  |      :  |
			 *  +---------+
			 *     [R2]   decorated_.size()
			 */
			
			assert(size().contains(c));

			// Is coordinate c in part A, B, C or D? (if existant)

			// if c is in A, B or C, add size() so it is in R2
			// if c is in D, nothing will happen
			// if c is neither in A, B, C or D it will land outside of R2

			if(c.column() < offset_.column()) {
				c += Coordinate<>(0, size().column());
			}

			if(c.row() < offset_.row()) {
				c += Coordinate<>(size().row(), 0);
			}

			// substract offset_ to make the coordinate relative to decorated_.
			// If it is inside, pass it through

			if(decorated_.size().contains(c - offset_)) {
				decorated_.set_pixel(c - offset_, color);
			}
		} // set_pixel()

		uint8_t get_pixel(Coordinate<> c) const {
			return decorated_.get_pixel((c + offset_) % decorated_.size());
		}

		void resize(Coordinate<> new_size) {
			size_ = new_size;
			offset_ %= size();
		}

		Coordinate<> size() const  { return size_; }

	private:
		TDecorated& decorated_;
		Coordinate<> size_;
		Coordinate<double> speed_;
		Coordinate<double> offset_;
};

#endif // SCROLLING_CANVAS_H
