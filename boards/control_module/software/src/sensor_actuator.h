#ifndef sensor_actuator_h_INCLUDED
#define sensor_actuator_h_INCLUDED

template<typename TIndex>
class SensorActuator {
	public:
		using Index = TIndex;

		enum {
			DATA_BITS = Index::MAX,
			DATA_BYTES = (Index::MAX +7) / 8
		};

		typedef std::bitset<DATA_BITS> Bitset;

		virtual void next_frame() { };
		virtual void set() = 0;
		virtual void set(Index i, bool v) = 0;
		virtual bool get(Index i) = 0;
		const Bitset& get_bits() = 0;
};

#endif // sensor_actuator_h_INCLUDED

