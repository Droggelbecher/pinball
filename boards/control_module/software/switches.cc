
#include "switches.h"
//#include "config.h"

//#include <checksum.h>

//#include <cstring> // memset
//#include <cassert>
//#include <cstdio>

Switches::Switches(Spi& spi) : spi(spi) {
}

//void Switches::read() {
	//static uint8_t request[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	//uint8_t answer[9];
	//memset(answer, 0x00, sizeof(answer));
	
	//spi.enable_slave(SPI_SS_PIN_SWITCHES);
	//spi.readwrite(sizeof(answer), request, answer);

	//if(checksum(answer, sizeof(answer) - 1) == answer[sizeof(answer) - 1]) {
		//assert(sizeof(answer) - 1 == sizeof(state_));
		//memcpy(state_, answer, sizeof(answer) - 1);
		//[>printf("ok sw %02x %02x %02x != %02x\n", answer[0], answer[1], answer[8], checksum(answer, 8));<]
		//fflush(stdout);
	//}
	//else {
//#if SWITCHES_PRINT_ERRORS
		//printf("err sw %02x %02x %02x %02x %02x %02x %02x %02x %02x != %02x\n", answer[0], answer[1], answer[2], answer[3], answer[4], answer[5], answer[6], answer[7], answer[8], checksum(answer, 8));
		//fflush(stdout);
//#endif
	//}

	//spi.disable_slaves();
//}

//bool Switches::get(Index index) {
	//int idx = static_cast<int>(index);
	//return !(state_[idx / 8] & (1 << (idx % 8)));
//}

//bool Switches::all(Index index, unsigned range) {
	//int idx = static_cast<int>(index);
	//assert((idx % 8 + range) < 8);
	//const uint8_t mask = (1 << range) - 1;
	//const uint8_t v = (~state_[idx / 8] & (mask << (idx % 8)));
	//return v != 0;
//}


#include <cassert>
#include <vector>
#include <bitset>

#include <iostream>

#include "utils.h"

namespace {

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
	else {
		std::cout << "sw chksum fail" << std::endl;
	}
}

bool Switches::get(Index index) {
	return bits[(int)index];
}



