
#include <fcntl.h>
#include <linux/spi/spidev.h>
#include <linux/types.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define GPIO_SET_INPUT(g) *(gpio_map+((g)/10)) &= ~(7<<(((g)%10)*3))
#define GPIO_SET_OUTPUT(g) *(gpio_map+((g)/10)) |=  (1<<(((g)%10)*3))
#define GPIO_SET *(gpio_map+7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR *(gpio_map+10) // clears bits which are 1 ignores bits which are 0
#define GPIO_GET *(gpio_map+13)

enum {
	SPI_BUS_SPEED     = (1000 * 1000),
	SPI_MODE          = 0,
	SPI_BITS_PER_WORD = 8,
	BCM2708_PERI_BASE = 0x20000000,
	GPIO_BASE         = (BCM2708_PERI_BASE + 0x200000), /* GPIO controller */
	GPIO_PAGE_SIZE    = (4*1024),
	GPIO_BLOCK_SIZE   = (4*1024)
};

volatile uint32_t *gpio_map = NULL;
int spi_cs_fd = -1;

int open_gpio() {
	void *m;

	int gpio_mem_fd = open("/dev/mem", O_RDWR | O_SYNC);
	if(gpio_mem_fd < 0) {
		perror("cannot open /dev/mem");
		exit(1);
		/*return errno;*/
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
		/*return errno;*/
	}
	gpio_map = (volatile uint32_t*)m;
	return 0;
}

void close_gpio() {
	if(gpio_map != NULL) {
		munmap((void*)gpio_map, GPIO_BLOCK_SIZE);
		gpio_map = NULL;
	}
}


int open_spi() {
	int status = -1;
	unsigned char spi_mode = SPI_MODE;
	unsigned char spi_bits_per_word = SPI_BITS_PER_WORD;
	unsigned spi_speed = SPI_BUS_SPEED;

	int spi_cs_fd = open("/dev/spidev0.0", O_RDWR);
	if(spi_cs_fd < 0) {
		perror("Couldnt open SPI device");
		exit(1);
	}

	status = ioctl(spi_cs_fd, SPI_IOC_WR_MODE, &spi_mode);
	if(status < 0) {
		perror("Could not set SPI write mode");
		exit(1);
	}

	status = ioctl(spi_cs_fd, SPI_IOC_RD_MODE, &spi_mode);
	if(status < 0) {
		perror("Could not set SPI read mode");
		exit(1);
	}

	status = ioctl(spi_cs_fd, SPI_IOC_WR_BITS_PER_WORD, &spi_bits_per_word);
	if(status < 0) {
		perror("Couldnt set bits per word (WR)");
		exit(1);
	}

	status = ioctl(spi_cs_fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed);
	if(status < 0) {
		perror("Couldnt set max write speed");
		exit(1);
	}

	status = ioctl(spi_cs_fd, SPI_IOC_RD_MAX_SPEED_HZ, &spi_speed);
	if(status < 0) {
		perror("Couldnt set max read speed");
		exit(1);
	}
	return spi_cs_fd;
}

void close_spi(int fd) {
	close(fd);
}

void gpio_set_output(int n) {
	GPIO_SET_INPUT(n);
	GPIO_SET_OUTPUT(n);
}

void gpio_enable_only(uint32_t slave_mask, uint32_t all_mask) {
	/*GPIO_SET = ALL_SLAVES;*/
	/*GPIO_CLR = (1 << (int)slave);*/
	GPIO_SET = all_mask;
	GPIO_CLR = slave_mask;
	usleep(100UL);
}

void gpio_disable_all(uint32_t all_mask) {
	GPIO_CLR = 0;
	GPIO_SET = all_mask;
	usleep(100UL);
}

int spi_transfer(int fd, int size, unsigned char *wr, unsigned char *rd) {
	struct spi_ioc_transfer msg;

	memset((void*)&msg, 0x00, sizeof(msg));

	msg.tx_buf = (unsigned long)wr;
	msg.rx_buf = (unsigned long)rd;
	msg.len = size;
	msg.delay_usecs = 0;
	msg.speed_hz = SPI_BUS_SPEED;
	msg.bits_per_word = 8;
	msg.cs_change = 0;
	msg.pad = 0;

	int r = ioctl(fd, SPI_IOC_MESSAGE(1), &msg);

	if(r == -1) {
		perror("couldnt send spi msg");
		exit(1);
	}

	return 0;
}


