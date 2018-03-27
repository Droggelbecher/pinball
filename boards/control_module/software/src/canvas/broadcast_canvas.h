
#ifndef BROADCAST_CANVAS_H
#define BROADCAST_CANVAS_H

#include <vector>
#include <memory>
#include <functional>

template<typename... Cs>
class BroadcastCanvas {
	public:
		void clear() { };
		void set_pixel(Coordinate<>, uint8_t) { };
		void next_frame(double) { };
};

template<typename C, typename... Cs>
class BroadcastCanvas<C, Cs...> : BroadcastCanvas<Cs...> {
		using Parent = BroadcastCanvas<Cs...>;

	public:
		BroadcastCanvas(C& canvas, Cs&... cs)
			: Parent(cs...), canvas_(canvas) { }

		void clear() {
			canvas_.clear();
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

	private:
		//std::vector<std::reference_wrapper<Canvas> > canvas_;
		C& canvas_;
};

#endif // BROADCAST_CANVAS_H

