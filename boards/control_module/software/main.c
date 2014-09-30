
#include "gpio.h"
#include "spi.h"

int main(int argc, char **argv) {
	gpio_setup();
	spi_setup();

	return 0;
}


