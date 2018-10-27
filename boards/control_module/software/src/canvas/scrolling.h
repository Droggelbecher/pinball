#ifndef SCROLLING_BUFFER_H
#define SCROLLING_BUFFER_H

#include "coordinate.h"
#include "canvas.h"

namespace pinball {
namespace canvas {

	template<typename TDecorated>
	class Scrolling {

		private:

			template<typename T>
			auto next_frame_(double dt, int) -> decltype(T().next_frame(dt), void()) {
				decorated_.next_frame(dt);
			}

			template<typename T>
			void next_frame_(double dt, long) {
			}

		public:

			static const DataOrder data_order = DataOrder::COLUMN_FIRST;

			Scrolling(TDecorated& decorated, Coordinate<double> speed)
				: decorated_(decorated), speed_(speed) {
			}

			void next_frame(double dt) {
				next_frame_<TDecorated>(dt, 0);
				offset_ += speed_ * dt;
				offset_ %= size();
			}

			Coordinate<> offset() const {
				return offset_;
			}

			Coordinate<> size() const {
				return decorated_.size();
			}

			void set_pixel(Coordinate<> c, uint8_t color) {
				decorated_.set_pixel((c + offset()) % size(), color);
			}

			uint8_t get_pixel(Coordinate<> c) const {
				return decorated_.get_pixel((c + offset()) % decorated_.size());
			}

			// Semantics of our buffer are a little different,
			// make sure we don't unintentionally use normal free functions for canvases here (by making buffer access private)
			// unless explicitely greenlighted by friends

			// We provide a specialized blit() that takes offset into account
			template<typename C, typename T>
			friend void blit(const Scrolling<T>&, C&, Coordinate<>, Coordinate<>, Coordinate<>);

			template<typename C, typename T>
			friend void blit(const C&, Scrolling<T>&, Coordinate<>, Coordinate<>, Coordinate<>);

			// clear() can not possibly do something wrong
			// (clearing the whole buffer by whatever means will always also clear the whole shifted buffer)
			template<typename C>
			friend void clear(C&);

			/*
			template<>
			auto buffer() -> decltype(TDecorated().buffer()) {
				return decorated_.buffer();
			}

			const uint8_t* buffer() const {
				return decorated_.buffer();
			}

			size_t buffer_length() const {
				return decorated_.buffer_length();
			}
			*/

		private:
			TDecorated& decorated_;
			Coordinate<double> speed_;
			Coordinate<double> offset_;
	};

	/**
	 * Blit this onto some other canvas C
	 */
	template<typename C, typename T>
	void blit(
		const Scrolling<T>& source, C& target,
		Coordinate<> start, Coordinate<> end, Coordinate<> offset
	) {
		interface().logger()
			<< "blit(Scr->, " << start << end << offset << ")\n";
		assert(source.size().contains(start));
		assert(source.size().contains(end));

		Coordinate<> from = start + source.offset();
		Coordinate<> to = end + source.offset();

		blit_rect(source, target, from, to, offset);
	}

	template<typename C, typename T>
	void blit(
		const C& source, Scrolling<T>& target,
		Coordinate<> start, Coordinate<> end, Coordinate<> offset
	) {
		interface().logger()
			<< "blit(->Scr, " << start << end << offset << ")\n";
		blit(source, target.decorated_, start, end, offset);
	}

	namespace scrolling_detail {

		template<typename Ca, typename Cb>
		void blit_rect(
			const Ca& source, Cb& target,
			Coordinate<> from, Coordinate<> to, Coordinate<> offset
		) {

			// Figure out which parts of overlap we have.
			// In principle there can be 9 kinds of overlap
			// (too high/low column/row values & combinations thereof).
			// Treat this divide & conquer style by splitting the
			// requested rectangle with recursive calls until only
			// non-overlapping ones are left.

			Coordinate<> sz = source.size();

			if(from.row() < 0) {
				blit_rect(
					source, target,
					{ sz.row() - from.row(), from.column() },
					{ sz.row(), to.column() },
					offset
				);
				blit_rect(source, target, { 0, from.column() }, to, offset);
			}
			else if(to.row() > sz.row()) {
				blit_rect(
					source, target,
					{ 0, from.column() },
					{ to.row() - sz.row(), to.column() },
					offset
				);
				blit_rect(source, target, from, { sz.row(), to.column() }, offset);
			}
			else if(from.row() < 0) {
				blit_rect(
					source, target,
					{ from.row(), sz.column() - from.column() },
					{ to.row(), sz.column() },
					offset
				);
				blit_rect(source, target, { from.row(), 0 }, to, offset);
			}
			else if(to.column() > sz.column()) {
				blit_rect(
					source, target,
					{ from.row(), 0 },
					{ to.row(), to.column() - sz.column() },
					offset
				);
				blit_rect(source, target, from, { to.row(), sz.column() }, offset);
			}
			else {
				// rectangle is actually fully inside the buffer, yay!
				blit<Ca, Cb>(
					source, target,
					from, to,
					offset
				);
			}

		}
	} // ns scrolling_detail

} } // ns pinball::canvas

#endif // SCROLLING_BUFFER_H

