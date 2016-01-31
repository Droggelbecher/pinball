
#include "bit_actuator.h"

#include "checksum.h"

template<typename Index_, Spi::Slave SLAVE_, int N>
void BitActuator<Index_, SLAVE_, N>::next_frame() {
	state[N] = checksum(state, N);
	spi.transfer_and_check(SLAVE, std::vector<uint8_t>(state, state + N));
}

template<typename Index_, Spi::Slave SLAVE_, int N>
void BitActuator<Index_, SLAVE_, N>::set(Index index, bool value) {
	int idx = static_cast<int>(index);
	if(value) {
		state[idx / 8] &= ~(1 << (idx % 8));
	}
	else {
		state[idx / 8] |= 1 << (idx % 8);
	}
}


