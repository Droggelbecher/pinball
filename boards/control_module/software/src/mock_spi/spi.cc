
#include "spi.h"

#include <checksum.h>
#include "utils.h"

#include <iostream>

Spi::Spi() {
}

std::vector<uint8_t> Spi::transfer_and_check(Slave slave, std::vector<uint8_t> input) {

	//std::cout << "transfer_and_check(" << slave << ", " << input << ")"
		//<< "  output=" << std::vector<uint8_t>(output, output + sizeof(output))
		//<< " s=" << (int)s 
		//<< " fd=" << spi_cs_fd
		//<< std::endl;
	//}

	return std::vector<uint8_t>(0, input.size() - 1);
}

