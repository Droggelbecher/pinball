
#include "spi_ss.h"

void spi_ss_activate_only(int n) {
	GPIO_SET = SPI_SS_SLAVES_MASK;
	GPIO_CLR = 1 << n;
	usleep(100);
}

void spi_ss_deactivate_all(void) {
	GPIO_SET = SPI_SS_SLAVES_MASK;
	GPIO_CLR = 0;
	usleep(100);
}

