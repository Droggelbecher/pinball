
#include "solenoids.h"

#include <checksum.h>

void Solenoids::next_frame() {
	state[2] = checksum(state, 2);

	spi.transfer_and_check(Spi::Slave::SOLENOIDS,
			std::vector<uint8_t>(state, state + 3));
}

void Solenoids::set(Index index, bool value) {
	//bits[(int)index] = value;
	int idx = (int)index;
	if(value) {
		state[idx / 8] &= ~(1 << (idx % 8));
	}
	else {
		state[idx / 8] |= 1 << (idx % 8);
	}
}


