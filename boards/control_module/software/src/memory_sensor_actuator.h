#ifndef MEMORY_SENSOR_H_INCLUDED
#define MEMORY_SENSOR_H_INCLUDED

#include <bitset>
#include <cassert>

template<typename TIndex, typename TSensorActuator>
class SensorActuatorOverride {
	public:
		using Index = TIndex;

		enum {
			DATA_BITS = Index::MAX,
			DATA_BYTES = (Index::MAX +7) / 8
		};

		typedef std::bitset<DATA_BITS> Bitset;

		SensorActuatorOverride(TSensorActuator& decorated)
			: decorated_(decorated) {

		}

		void next_frame() { }
		void set() {
			bits.set();
		}
		void set(Index i, bool v) {
			bits[(int)i] = v;
		}
		bool get(Index i) {
			return bits[(int)i] ^ decorated_.get(i);
		}
		const Bitset& get_bits() {
			return bits ^ decorated_.get(i);
		}

		TSensorActuator& decorated() {
			return decorated_;
		}

	private:
		TSensorActuator& decorated_;
		Bitset bits;
};

#endif // MEMORY_SENSOR_H_INCLUDED

