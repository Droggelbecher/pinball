
#include "switches.h"
#include "spi.h"

namespace pinball {
	Switches::Switches(Spi& spi) : spi(spi) {
	}
} // ns pinball

#include <cassert>
#include <vector>
#include <bitset>

#include <iostream>

#include "utils.h"

namespace pinball {

namespace switches_detail {

	// TODO: move this somewhere more central (utils?)

	template<int N>
	std::bitset<N> to_bitset(const std::vector<uint8_t>& bytes) {
		std::bitset<N> r;
		assert(bytes.size() * 8 == N);
		for(int i = 0; i < N; i++) {
			r[i] = (bytes[i / 8] >> (i % 8)) & 0x01;
		}
		return r;
	}
} // ns switches_detail

void Switches::next_frame(double dt) {
	read();
}

void Switches::read() {
	std::vector<uint8_t> answer = spi.transfer_and_check(Spi::Slave::SWITCHES, { 0, 0, 0, 0, 0, 0, 0, 0, 0 });

	if(answer.size() == 8) {
		this->bits = to_bitset<64>(answer);
	}
	else {
		// TODO add proper logging
		//std::cout << "sw chksum fail" << std::endl;
	}
}

bool Switches::get(Index index) {
	return !bits[static_cast<int>(index)];
}

} // ns pinball

