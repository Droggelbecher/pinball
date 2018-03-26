
#ifndef SPI_H
#define SPI_H

#include <vector>
#include <cstdint>

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define GPIO_SET_INPUT(g) *(gpio_map+((g)/10)) &= ~(7<<(((g)%10)*3))
#define GPIO_SET_OUTPUT(g) *(gpio_map+((g)/10)) |=  (1<<(((g)%10)*3))

#define GPIO_SET *(gpio_map+7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR *(gpio_map+10) // clears bits which are 1 ignores bits which are 0

#define GPIO_GET *(gpio_map+13)


class Spi {

	public:
		enum {
			SPI_BUS_SPEED = 1000 * 1000,
			SPI_MODE = 0,
			SPI_BITS_PER_WORD = 8
		};

		enum Slave {
			LAMPS = 22,
			SWITCHES = 23,
			DISPLAY = 24,
			SOLENOIDS = 25,

			ALL_SLAVES = (1 << LAMPS)
				| (1 << DISPLAY)
				| (1 << SWITCHES)
				| (1 << SOLENOIDS)
		};

	private:
		static const uint32_t BCM2708_PERI_BASE = 0x20000000;
		static const uint32_t GPIO_BASE = (BCM2708_PERI_BASE + 0x200000); /* GPIO controller */
		static const uint32_t GPIO_PAGE_SIZE = (4*1024);
		static const uint32_t GPIO_BLOCK_SIZE = (4*1024);

	public:
		Spi();
		
		std::vector<uint8_t> transfer_and_check(Slave slave, std::vector<uint8_t> input);

		void enable_only(Slave slave);
		void disable_all();
		int transfer(int size, unsigned char *wr, unsigned char *rd);

	private:

		void open_gpio();
		void open_spi();
		void gpio_set_output(int n);

		volatile uint32_t *gpio_map;
		int spi_cs_fd;
		int gpio_mem_fd;
};

#endif // SPI_H

