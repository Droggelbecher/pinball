
#include "bit_actuator.h"

#include "checksum.h"

namespace pinball {

template<typename Index_, Spi::Slave SLAVE_>
void BitActuator<Index_, SLAVE_>::next_frame(double dt) {
	state[DATA_BYTES] = checksum(state, DATA_BYTES);
	spi.transfer_and_check(SLAVE, std::vector<uint8_t>(state, state + DATA_BYTES + 1));
}

template<typename Index_, Spi::Slave SLAVE_>
void BitActuator<Index_, SLAVE_>::set(bool value) {
	memset(state, value ? 0x00 : 0xff, DATA_BYTES);
}

template<typename Index_, Spi::Slave SLAVE_>
void BitActuator<Index_, SLAVE_>::set(Index index, bool value) {
	int idx = static_cast<int>(index);
	if(value) {
		state[idx / 8] &= ~(1 << (idx % 8));
	}
	else {
		state[idx / 8] |= 1 << (idx % 8);
	}
}

template<typename Index_, Spi::Slave SLAVE_>
bool BitActuator<Index_, SLAVE_>::get(Index index) const {
	int idx = static_cast<int>(index);
	return !((state[idx / 8] >> (idx % 8)) & 0x01);
}

} // ns pinball

