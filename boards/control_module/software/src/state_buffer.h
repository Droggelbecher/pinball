
#ifndef STATE_BUFFER_H
#define STATE_BUFFER_H

template<typename T>
class StateBuffer {

	private:
		void swap_buffers() {
			state_idx = 1 - state_idx;
		}

		typename T::Bitset& current() {
			return state[state_idx];
		}

		typename T::Bitset& previous() {
			return state[1 - state_idx];
		}

	
	public:
		StateBuffer(T& t) : decorated(t) {
		}

		void next_frame(double dt) {
			swap_buffers();
			current() = decorated.get_bits();
			decorated.next_frame(dt);
		}

		bool changed() {
			return previous() != current();
		}

		bool rising(typename T::Index idx) {
			int i = static_cast<int>(idx);
			return !previous()[i] && current()[i];
		}

		bool falling(typename T::Index idx) {
			int i = static_cast<int>(idx);
			return previous()[i] && !current()[i];
		}

		const typename T::Bitset& get_previous_state() {
			return previous();
		}

	private:
		T& decorated;
		int state_idx = 0;
		typename T::Bitset state[2];
};

#endif // STATE_BUFFER_H

/* vim: set ts=2 sw=2 tw=78 noexpandtab :*/
