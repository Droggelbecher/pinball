
#include <fcntl.h>
#include <linux/spi/spidev.h>
#include <linux/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <time.h>
#include <unistd.h>

#include "config.h"
#include "spi.h"

int spi_cs_fd;

int spi_setup(void) {
	int status = -1;
	unsigned char spi_mode = SPI_MODE;
	unsigned char spi_bits_per_word = SPI_BITS_PER_WORD;
	unsigned spi_speed = SPI_BUS_SPEED;

	spi_cs_fd = open("/dev/spidev0.0", O_RDWR);
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

	return status;
}

int spi_readwrite(size_t size, unsigned char *wr, unsigned char *rd) {
	struct spi_ioc_transfer msg = {
		.tx_buf = (unsigned long)wr,
		.rx_buf = (unsigned long)rd,
		.len = size,
		.delay_usecs = 0,
		.speed_hz = SPI_BUS_SPEED,
		.bits_per_word = 8,
	};

	int r = ioctl(spi_cs_fd, SPI_IOC_MESSAGE(1), &msg);
	if(r == 1) {
		perror("couldnt send spi msg");
		exit(1);
	}

	usleep(100);

	return 0;
}


