
#include "spi_ss.h"

void spi_ss_activate_only(int n) {
	/*GPIO_SET = SPI_SS_SLAVES_MASK;*/
	/*printf("slavemask=%x\n", SPI_SS_SLAVES_MASK);*/
	GPIO_CLR = (1 << n);
	usleep(100);
}

void spi_ss_deactivate_all(void) {
	/*GPIO_CLR = 0;*/
	GPIO_SET = SPI_SS_SLAVES_MASK;
	usleep(100);
}

