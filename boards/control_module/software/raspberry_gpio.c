
#include "raspberry_gpio.h"

int gpio_mem_fd;
volatile unsigned *gpio_map;

void gpio_setup(void) {
	void *m;

	gpio_mem_fd = open("/dev/mem", O_RDWR | O_SYNC);
	if(gpio_mem_fd < 0) {
		perror("cannot open /dev/mem");
		exit(1);
	}

	m = mmap(
			0,
			GPIO_BLOCK_SIZE,
			PROT_READ | PROT_WRITE,
			MAP_SHARED,
			gpio_mem_fd,
			GPIO_BASE
	);

	close(gpio_mem_fd);

	if(m == MAP_FAILED) {
		perror("mmap failed");
		exit(1);
	}

	gpio_map = (volatile unsigned *)m;
}

void gpio_set_output(int n) {
	GPIO_SET_INPUT(n);
	GPIO_SET_OUTPUT(n);
}


