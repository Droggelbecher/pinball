#ifndef SENSOR_ACTUATOR_OVERRIDE_H_INCLUDED
#define SENSOR_ACTUATOR_OVERRIDE_H_INCLUDED

#include <bitset>
#include <cassert>

#include "memory_sensor_actuator.h"


/**
 * Decorate a given SensorActutator in the following way:
 * - set()ing is intercepted and sets the state of the decorator
 * - get()ing returns decorated XOR decorator iff enabled, else it shows state of decorated
 * 
 * Thus, enable_xor gives a masking behavior, whereas disabled will just
 * throw away all set() calls and pass through get().
 */
template<typename TSensorActuator>
class SensorActuatorOverride {
	public:
		using Index = typename TSensorActuator::Index;

		enum Mode {
			PASS_THROUGH, // set() -> decorated,      get() = decorated
			MASK_GET,     // set() -> internal state, get() = internal state XOR decorated
			READ_ONLY     // set() -> internal state, get() = decorated
		};

		enum {
			DATA_BITS = static_cast<int>(Index::MAX),
			DATA_BYTES = (DATA_BITS +7) / 8
		};

		typedef std::bitset<DATA_BITS> Bitset;

		SensorActuatorOverride(TSensorActuator& decorated, Mode mode = MASK_GET)
			: decorated_(decorated), mode_(mode) {

		}

		void set_mode(Mode mode) {
			mode_ = mode;
		}

		void next_frame(double dt) { }
		void set(bool v) {
			if(mode_ == PASS_THROUGH) {
				decorated_.set(v);
			}
			else {
				if(v) {
					bits.set();
				}
				else {
					bits.reset();
				}
			}
		}

		void set(Index i, bool v) {
			if(mode_ == PASS_THROUGH) {
				decorated_.set(i, v);
			}
			else {
				bits[(int)i] = v;
			}
		}

		bool get(Index i) {
			if(mode_ == MASK_GET) {
				return bits[(int)i] != decorated_.get(i);
			}
			else {
				return decorated_.get(i);
			}
		}

		const Bitset get_bits() {
			if(mode_ == MASK_GET) {
				return bits ^ decorated_.get_bits();
			}
			else {
				return decorated_.get_bits();
			}
		}

		TSensorActuator& decorated() {
			return decorated_;
		}

	private:
		TSensorActuator& decorated_;
		Mode mode_;
		Bitset bits;
};

#endif // MEMORY_SENSOR_H_INCLUDED

