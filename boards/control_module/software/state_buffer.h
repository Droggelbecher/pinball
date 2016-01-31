
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

		bool rising(typename T::Index idx) {
			return !previous_state[idx] && decorated.get(idx);
			
		}

		bool falling(typename T::Index idx) {
			return previous_state[idx] && !decorated.get(idx);
		}

	private:
		T& decorated;
		typename T::Bitset previous_state;
};

#endif // STATE_BUFFER_H

