
#ifndef BROADCAST_CANVAS_H
#define BROADCAST_CANVAS_H

#include <vector>
#include <memory>
#include <functional>
#include "coordinate.h"
#include "canvas.h"
#include "canvas/scrolling.h"
#include "assertions.h"

#include "main.h"

namespace pinball { namespace canvas {

template<typename... Cs>
class BroadcastCanvas {
	public:
		static const DataOrder data_order = DataOrder::NONE;

		void clear() { };
		void set_pixel(Coordinate<>, uint8_t) { };
		void next_frame(double) { };

	protected:
		template<typename CC>
		void blit_onto(const CC& source, Coordinate<> from, Coordinate<> to, Coordinate<> offset) {
		}
};

template<typename C, typename... Cs>
class BroadcastCanvas<C, Cs...> : BroadcastCanvas<Cs...> {
		using Parent = BroadcastCanvas<Cs...>;

		static const DataOrder data_order =
			(
			    (C::data_order == Parent::data_order)
			 || (Parent::data_order == DataOrder::NONE)
			)
			? C::data_order
			: DataOrder::OTHER;

	public:
		BroadcastCanvas(C& canvas, Cs&... cs)
			: Parent(cs...), canvas_(canvas) { }

		void clear() {
			canvas::clear(canvas_);
			Parent::clear();
		}

		void set_pixel(Coordinate<> c, uint8_t color) {
			canvas_.set_pixel(c, color);
			Parent::set_pixel(c, color);
		}

		uint8_t get_pixel(Coordinate<> c) const {
			return canvas_.get_pixel(c);
		}

		void next_frame(double dt) {
			canvas_.next_frame(dt);
			Parent::next_frame(dt);
		}

		Coordinate<> size() const {
			return canvas_.size();
		}

		template<typename C_, typename BC_, typename BCs_>
		friend void blit(const C_&, BroadcastCanvas<BC_, BCs_>&, Coordinate<>, Coordinate<>, Coordinate<>);

	protected:
		template<typename CC>
		void blit_onto(const CC& source, Coordinate<> from, Coordinate<> to, Coordinate<> offset) {
			blit(source, canvas_, from, to, offset);
			Parent::blit_onto(source, from, to, offset);
		}

	private:
		C& canvas_;
};

template<typename C, typename BC, typename BCs>
void blit(
		const C& source, BroadcastCanvas<BC, BCs>& target,
		Coordinate<> from, Coordinate<> to, Coordinate<> offset
) {
	assert_blittable(source, target, from, to, offset);
	//interface().logger()
		//<< "blit(->BC, " << from << to << offset << ")\n";
	target.blit_onto(source, from, to, offset);
}


} } // Pinball::canvas

#endif // BROADCAST_CANVAS_H

