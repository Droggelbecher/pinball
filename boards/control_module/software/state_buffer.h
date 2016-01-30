
#ifndef STATE_BUFFER_H
#define STATE_BUFFER_H

template<typename T>
class StateBuffer {
	
	public:
		StateBuffer(T& t) : decorated(t) {
		}

		void next_frame() {
			/*
			 * a) direct copy of internal storage. Efficient & simple, but
			 * leaks internal storage details of wrapped classes (templated
			 * friends?)
			 *
			 * this->old_state of type typename T::InternalStorage.
			 * this->old_state = t.get_internal_storage()
			 *
			 * how to recover old state for comparison?
			 * -> a.1) define it to always be a bitmask with according indices
			 *  (so it would more be a t.get_bits() that a
			 *  t.get_internal_storage())
			 *  ==> EFFICIENT, LIMITED
			 *
			 * -> a.2) T or the object itself provides additional methods for
			 *  getting the bits
			 *  ==> COMPLICATED
			 *
			 * b) bit-vector.
			 * this->old_state of type vector<bool>
			 * for(...) {
			 *   this->old_state[i] = t.get(i);
			 * }
			 * ==> INEFFICIENT, STRAIGHT-FORWARD, relatively FLEXIBLE
			 *
			 *
			 * c) copy the whole damn object
			 * ==> INEFFICIENT (but in other ways), STRAIGHT-FORWARD,
			 * relatively FLEXIBLE
			 */

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

