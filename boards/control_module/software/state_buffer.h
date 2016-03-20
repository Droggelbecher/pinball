
#ifndef STATE_BUFFER_H
#define STATE_BUFFER_H

template<typename T>
class StateBuffer {
	
	public:
		StateBuffer(T& t) : decorated(t) {
		}

		void next_frame() {
			previous_state = decorated.get_bits();
			decorated.next_frame();
		}

		bool changed() {
			return previous_state != decorated.get_bits();
		}

		bool rising(typename T::Index idx) {
			return !previous_state[idx] && decorated.get(idx);
			
		}

		bool falling(typename T::Index idx) {
			return previous_state[idx] && !decorated.get(idx);
		}

		const typename T::Bitset& get_previous_state() {
			return previous_state;
		}

	private:
		T& decorated;
		typename T::Bitset previous_state;
};

#endif // STATE_BUFFER_H

