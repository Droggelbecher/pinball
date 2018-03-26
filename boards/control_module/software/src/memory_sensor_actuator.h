#ifndef MEMORY_SENSOR_H_INCLUDED
#define MEMORY_SENSOR_H_INCLUDED

#include <bitset>
#include <cassert>


template<typename TIndex>
class MemorySensorActuator {
	public:
		using Index = TIndex;

		enum {
			DATA_BITS = static_cast<int>(Index::MAX),
			DATA_BYTES = (DATA_BITS +7) / 8
		};

		typedef std::bitset<DATA_BITS> Bitset;

		void next_frame(double dt) { }

		void set(bool v) {
			if(v) {
				bits.set();
			}
			else {
				bits.reset();
			}
		}
		void set(Index i, bool v) {
			bits[(int)i] = v;
		}
		bool get(Index i) {
			return bits[(int)i];
		}
		const Bitset& get_bits() {
			return bits;
		}

	private:
		Bitset bits;
};

#endif // MEMORY_SENSOR_H_INCLUDED

