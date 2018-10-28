#ifndef SCROLLING_BUFFER_H
#define SCROLLING_BUFFER_H

#include <iostream>

#include "coordinate.h"
#include "canvas.h"
#include "assertions.h"

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
				offset_ %= virtual_size();
			}

			Coordinate<> offset() const {
				return offset_;
			}

			Coordinate<> size() const {
				return decorated_.size();
			}

			Coordinate<> virtual_size() const {
				return { 11, 80 };
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

			TDecorated& decorated() { return decorated_; }

		private:
			TDecorated& decorated_;
			Coordinate<double> speed_;
			Coordinate<double> offset_;
	};

	/*
	template<typename C, typename T>
	void blit(
		const C& source, Scrolling<T>& target,
		Coordinate<> start, Coordinate<> end, Coordinate<> offset
	) {
		//interface().logger()
			//<< "blit(->Scr, " << start << end << offset << ")\n";
		blit(source, target.decorated_, start, end, offset);
	}
	*/

	namespace scrolling_detail {

		int min(int a, int b) { return (a < b) ? a : b; }
		int max(int a, int b) { return (a > b) ? a : b; }

		template<typename T>
		void blit_crop(
				Coordinate<> begin_a,
				Coordinate<> end_a,
				Coordinate<> begin_b,
				Coordinate<> size_b,
				T f
		) {
				//std::cout << "call begin_a=" << begin_a
					//<< " end_a=" << end_a
					//<< " begin_b=" << begin_b
					//<< " size_b=" << size_b
					//<< "\n";

			begin_a = Coordinate<> { max(0, begin_a.row()),	max(0, begin_a.column()) };
			end_a = Coordinate<> { max(0, end_a.row()),	max(0, end_a.column()) };
			begin_b = Coordinate<> { max(0, begin_b.row()),	max(0, begin_b.column()) };

			begin_a = end_a.crop(begin_a);
			Coordinate<> sz_a = end_a - begin_a;

			if(!sz_a.area()) {
				return;
			}

			Coordinate<> end_b = begin_b + sz_a;

			begin_b = end_b.crop(begin_b);

			Coordinate<> offset = begin_b - begin_a;

			if(begin_b.row() < 0) {
				return blit_crop(
						{ 0 - offset.row(), begin_a.column() },
						end_a,
						{ 0, begin_b.column() },
						size_b, f
						);
			}
			if(end_b.row() > size_b.row()) {
				return blit_crop(
						begin_a,
						{ size_b.row() - offset.row(), end_b.column() },
						begin_b, size_b, f
						);
			}
			if(begin_b.column() < 0) {
				return blit_crop(
						{ begin_a.row(), 0 - offset.column() },
						end_a,
						{ begin_b.row(), 0 },
						size_b, f
						);
			}
			if(end_b.column() > size_b.column()) {
				//std::cout << "=> begin_a=" << begin_a
					//<< " end_a=" << end_a
					//<< " sz_a=" << sz_a
					//<< " begin_b=" << begin_b
					//<< " end_b=" << end_b
					//<< " size_b=" << size_b
					//<< " offs=" << offset
					//<< "\n";
				return blit_crop(
						begin_a,
						{ end_b.row(), size_b.column() - offset.column() },
						begin_b, size_b, f
						);
			}

			//std::cout << "=> " << begin_a << end_a << begin_b << "\n";
			f(begin_a, end_a, begin_b);
		}

		template<typename T>
		void blit_wrap(
				Coordinate<> begin_b,
				Coordinate<> end_b,
				Coordinate<> begin_c,
				Coordinate<> size_b,
				Coordinate<> size_c,
				T f
		) {
			blit_crop(begin_b, end_b, begin_c + begin_b, size_c, f);
			blit_crop(begin_b, end_b, begin_c + begin_b - size_b, size_c, f);
			blit_crop(begin_b, end_b, begin_c + begin_b - Coordinate<>(0, size_b.column()), size_c, f);
		}

		template<typename C, typename T>
		void blit_scroll(const C& ca, Scrolling<T>& cb, Coordinate<> begin_a, Coordinate<> end_a, Coordinate<> begin_b) {
			blit_crop(
					begin_a, end_a,
					begin_b, cb.virtual_size(),
					// Crop blit from a -> virtual space as necessary
					// Result is coordinates in a (ba/ea) and virtual space(bb)
					[&](Coordinate<> ba, Coordinate<> ea, Coordinate<> bb) {
						// wrap result into target
						blit_wrap(
								bb, bb + ea - ba, cb.offset(), cb.virtual_size(), cb.size(),
								[&](Coordinate<> bb2, Coordinate<> eb2, Coordinate<> bc2) {
									assert_blittable(
											ca, cb.decorated(),
											begin_a, begin_a + eb2 - bb2, bc2
									);
									blit(
											ca, cb.decorated(),
											begin_a, begin_a + eb2 - bb2,
											bc2
									);
								}
						); // blit_wrap
					}
			); // blit_crop
		}


		template<typename Ca, typename Cb>
		void blit_rect_(
			const Ca& source, Cb& target,
			Coordinate<> from, Coordinate<> to, Coordinate<> tgt
		) {
			assert(source.size().contains(from));
			assert(source.size().contains(to));

			// Figure out which parts of overlap we have.
			// In principle there can be 9 kinds of overlap
			// (too high/low column/row values & combinations thereof).
			// Treat this divide & conquer style by splitting the
			// requested rectangle with recursive calls until only
			// non-overlapping ones are left.

			Coordinate<> sz = target.size();
			Coordinate<> span = to - from;
			Coordinate<> tgt_end = tgt + span;

			if(tgt.row() < 0) {
				blit_rect(
						source, target,
						from,
						{ from.row() - tgt.row(), to.column() },
						{ sz.row() + tgt.row(), tgt.column() }
				);

				blit_rect(
						source, target,
						{ from.row() - tgt.row(), from.column() },
						to,
						{ 0, tgt.column() }
				);
			}

			else if(tgt_end.row() > sz.row()) {
				blit_rect(
						source, target,
						from,
						{ to.row() + sz.row() - tgt.row(), to.column() },
						tgt
				);

				blit_rect(
						source, target,
						{ from.row() + sz.row() - tgt.row(), from.column() },
						to,
						{ 0, tgt.column() }
				);
			}

			else if(tgt.column() < 0) {
				blit_rect(
						source, target,
						from,
						{ to.row(), from.column() - tgt.column() },
						{ tgt.row(), sz.column() + tgt.column() }
				);

				blit_rect(
						source, target,
						{ from.row(), from.column() - tgt.column() },
						to,
						{ tgt.row(), 0 }
				);
			}

			else if(tgt_end.column() > sz.column()) {
				blit_rect(
						source, target,
						from,
						{ to.row(), to.column() + sz.column() - tgt.column() },
						tgt
				);

				blit_rect(
						source, target,
						{ from.row(), from.column() + sz.column() - tgt.column() },
						to,
						{ tgt.row(), 0 }
				);
			}

			else {
				assert_blittable(source, target.decorated(), from, to, tgt);
				// rectangle is actually fully inside the buffer, yay!
				if((to - from).area()) {
					blit(
						source, target.decorated(),
						from, to,
						tgt
					);
				}
			}

		}
	} // ns scrolling_detail

	/**
	 * Blit onto the scrolling canvas
	 */
	template<typename C, typename T>
	void blit(
		const C& source,
		Scrolling<T>& target,
		Coordinate<> start, Coordinate<> end, Coordinate<> offset
	) {
		//interface().logger()
			//<< "blit(Scr->, " << start << end << offset << ")\n";
		//assert(source.size().contains(start));
		//assert(source.size().contains(end));

		//offset += target.offset();
		//Coordinate<> to = end + target.offset();

		//assert(false);
		//scrolling_detail::blit_rect(source, target, start, end, offset + target.offset());
		scrolling_detail::blit_scroll(source, target, start, end, offset); //, offset + target.offset());
	} // blit

} } // ns pinball::canvas

#endif // SCROLLING_BUFFER_H

