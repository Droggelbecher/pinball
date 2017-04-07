#ifndef MEMORY_SENSOR_H_INCLUDED
#define MEMORY_SENSOR_H_INCLUDED

#include <bitset>
#include <cassert>

template<typename TIndex>
class MemorySensorActuator {
	public:
		using Index = TIndex;

		enum {
			DATA_BITS = Index::MAX,
			DATA_BYTES = (Index::MAX +7) / 8
		};

		typedef std::bitset<DATA_BITS> Bitset;

		void next_frame() { }
		void set() {
			bits.set();
		}
		void set(Index i, bool v) {
			bits[(int)i] = v;
		}
		bool get(Index i) { return bits[(int)i]; }
		const Bitset& get_bits() { return bits; }
	private:
		Bitset bits;
};

#endif // MEMORY_SENSOR_H_INCLUDED

