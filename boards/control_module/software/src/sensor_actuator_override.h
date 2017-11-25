#ifndef SENSOR_ACTUATOR_OVERRIDE_H_INCLUDED
#define SENSOR_ACTUATOR_OVERRIDE_H_INCLUDED

#include <bitset>
#include <cassert>

#include "memory_sensor_actuator.h"


/**
 * Decorate a given SensorActutator in the following way:
 * - set()ing is intercepted and sets the state of the decorator
 * - get()ing returns the state of decorated XOR state of of decorator.
 */
template<typename TSensorActuator>
class SensorActuatorOverride {
	public:
		using Index = typename TSensorActuator::Index;

		enum {
			DATA_BITS = static_cast<int>(Index::MAX),
			DATA_BYTES = (DATA_BITS +7) / 8
		};

		typedef std::bitset<DATA_BITS> Bitset;

		SensorActuatorOverride(TSensorActuator& decorated)
			: decorated_(decorated) {

		}

		void next_frame(double dt) { }
		void set() {
			bits.set();
		}
		void set(Index i, bool v) {
			bits[(int)i] = v;
		}
		bool get(Index i) {
			return bits[(int)i] ^ decorated_.get(i);
		}
		const Bitset get_bits() {
			return bits ^ decorated_.get_bits();
		}

		TSensorActuator& decorated() {
			return decorated_;
		}

	private:
		TSensorActuator& decorated_;
		Bitset bits;
};

#endif // MEMORY_SENSOR_H_INCLUDED

