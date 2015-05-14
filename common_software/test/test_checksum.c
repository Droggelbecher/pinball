
#undef NDEBUG
#include <assert.h>
#include <stdio.h>

#include "../include/checksum.h"


int main(int argc, char **argv) {
	// for zero initialization and no inversion at end, this can be used for
	// verification:
	// http://depa.usst.edu.cn/chenjq/www2/software/crc/CRC_Javascript/CRCcalculation.htm
	// (however, we do both of those ^^)

	{
		uint8_t data[3] = { 0x57 }; // wikipedia example
		printf("%02x\n", checksum(data, 1));
		assert(checksum(data, 1) == 0x29);
	}

	{
		uint8_t data[] = { 0xaa }; 
		printf("%02x\n", checksum(data, 1));
		assert(checksum(data, 1) == 0x05);
	}

	{
		uint8_t data[] = { 0xaa, 0xaa }; 
		printf("%02x\n", checksum(data, sizeof(data)));
		assert(checksum(data, sizeof(data)) == 0x93);
	}

	{
		uint8_t data[] = { 0xff }; 
		printf("%02x\n", checksum(data, sizeof(data)));
		assert(checksum(data, sizeof(data)) == 0xff);
	}

	{
		uint8_t data[] = { 0xff, 0xff }; 
		printf("%02x\n", checksum(data, sizeof(data)));
		assert(checksum(data, sizeof(data)) == 0x30);
	}

	{
		uint8_t data[] = { 0xff, 0xff, 0xff, 0xff }; 
		printf("%02x\n", checksum(data, sizeof(data)));
		assert(checksum(data, sizeof(data)) == 0x0f);
	}

	{
		uint8_t data[] = { 0x0 }; 
		printf("%02x\n", checksum(data, sizeof(data)));
		assert(checksum(data, sizeof(data)) == 0x30);
	}

	{
		uint8_t data[] = { 0x00, 0x00 }; 
		printf("%02x\n", checksum(data, sizeof(data)));
		assert(checksum(data, sizeof(data)) == 0x14);
	}

	{
		uint8_t data[] = { 0x0, 0x0, 0x0, 0x0, 0x0 };
		printf("%02x\n", checksum(data, sizeof(data)));
		assert(checksum(data, sizeof(data)) == 0x63);
	}

	{
		uint8_t data[] = { 0x07 }; 
		printf("%02x\n", checksum(data, sizeof(data)));
		assert(checksum(data, sizeof(data)) == 0x45);
	}

	{
		uint8_t data[] = { 0xe0 }; 
		printf("%02x\n", checksum(data, sizeof(data)));
		assert(checksum(data, sizeof(data)) == 0x98);
	}

	{
		uint8_t data[] = { ~0x07 }; 
		printf("%02x\n", checksum(data, sizeof(data)));
		assert(checksum(data, sizeof(data)) == 0x8a);
	}

	{
		uint8_t data[] = { 0xe0 ^ 0xff }; 
		printf("%02x\n", checksum(data, sizeof(data)));
		assert(checksum(data, sizeof(data)) == 0x57);
	}
	return 0;
}


