
#include "switches.h"

#include <cassert>
#include <vector>
#include <bitset>

#include <iostream>

namespace {

	// TODO: move this somewhere more central (utils?)

	template<int N>
	std::bitset<N> to_bitset(const std::vector<uint8_t>& bytes) {
		std::bitset<N> r;
		std::cout << "N=" << (int)N << " bytes=" << (int)bytes.size() << std::endl;
		assert(bytes.size() * 8 == N);
		for(int i = 0; i < N; i++) {
			r[i] = (bytes[i / 8] >> (i % 8)) & 0x01;
		}
		return r;
	}
}

void Switches::next_frame() {
	read();
}

void Switches::read() {
	//static uint8_t request[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	//uint8_t answer[sizeof(request)];
	//memset(answer, 0x00, sizeof(answer));

	std::vector<uint8_t> answer = spi.transfer_and_check(Spi::Slave::SWITCHES, { 0, 0, 0, 0, 0, 0, 0, 0, 0 });

	if(answer.size() == 8) {
		this->bits = to_bitset<64>(answer);
	}
}

	
	//if(checksum(answer.begin(), answer.end()) == answer[sizeof(answer) - 1]) {
		//assert(sizeof(answer) - 1 == sizeof(switches_state_));
		//memcpy(switches_state_, answer, sizeof(answer) - 1);
		//[>printf("ok sw %02x %02x %02x != %02x\n", answer[0], answer[1], answer[8], checksum(answer, 8));<]
		//fflush(stdout);
	//}
	//else {
//#if SWITCHES_PRINT_ERRORS
		//printf("err sw %02x %02x %02x %02x %02x %02x %02x %02x %02x != %02x\n", answer[0], answer[1], answer[2], answer[3], answer[4], answer[5], answer[6], answer[7], answer[8], checksum(answer, 8));
		//fflush(stdout);
//#endif
	//}

	//spi_ss_deactivate_all();



bool Switches::get(Index index) {
	return bits[(int)index];
}



