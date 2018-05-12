#ifndef sensor_actuator_h_INCLUDED
#define sensor_actuator_h_INCLUDED

namespace pinball {

template<typename TIndex>
class SensorActuator {
	public:
		using Index = TIndex;

		enum {
			DATA_BITS = Index::MAX,
			DATA_BYTES = (Index::MAX +7) / 8
		};

		typedef std::bitset<DATA_BITS> Bitset;

		virtual void next_frame(double dt) { };
		virtual void set(bool v) = 0;
		virtual void set(Index i, bool v) = 0;
		virtual bool get(Index i) = 0;
		const Bitset& get_bits() = 0;
};

} // ns pinball

#endif // sensor_actuator_h_INCLUDED

