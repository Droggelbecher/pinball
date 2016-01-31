
#include "spi.h"

#include "checksum.h"
#include "utils.h"

#include <fcntl.h>
#include <linux/spi/spidev.h>
#include <linux/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <time.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

extern "C" {
	volatile unsigned *gpio_map;
}


Spi::Spi() {
	open_gpio();
	gpio_set_output(LAMPS);
	gpio_set_output(SWITCHES);
	gpio_set_output(DISPLAY);
	gpio_set_output(SOLENOIDS);
	open_spi();
}

void Spi::gpio_set_output(int n) {
	GPIO_SET_INPUT(n);
	GPIO_SET_OUTPUT(n);
}

void Spi::open_gpio() {
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

	std::cout << "gpio set up." << std::endl;
}

void Spi::open_spi() {
	int status = -1;
	unsigned char spi_mode = SPI_MODE;
	unsigned char spi_bits_per_word = SPI_BITS_PER_WORD;
	unsigned spi_speed = SPI_BUS_SPEED;

	// TODO: make this more c++ish (exceptions)

	spi_cs_fd = ::open("/dev/spidev0.0", O_RDWR);
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

	std::cout << "spi open " << spi_cs_fd << std::endl;
}

std::vector<uint8_t> Spi::transfer_and_check(Slave slave, std::vector<uint8_t> input) {
	enable_only(slave);

	uint8_t output[input.size()];
	memset(output, 77, sizeof(output));


	this->transfer(input.size(), input.data(), output);
	disable_all();

	uint8_t s = checksum(output, sizeof(output) - 1);

	//if(slave != 25) {
	//std::cout << "transfer_and_check(" << slave << ", " << input << ")"
		//<< "  output=" << std::vector<uint8_t>(output, output + sizeof(output))
		//<< " s=" << (int)s 
		//<< " fd=" << spi_cs_fd
		//<< std::endl;
	//}

	if(s == output[sizeof(output) - 1]) {
		return std::vector<uint8_t>(output, output + sizeof(output) - 1);
	}
	else {
	//std::cout << "checksum fail transfer_and_check(" << slave << ", " << input << ")"
		//<< "  output=" << std::vector<uint8_t>(output, output + sizeof(output))
		//<< " s=" << (int)s 
		//<< " fd=" << spi_cs_fd
		//<< std::endl;
		return std::vector<uint8_t>();
	}
}

void Spi::enable_only(Slave slave) {
	GPIO_SET = ALL_SLAVES;
	GPIO_CLR = (1 << (int)slave);
	usleep(100UL);
}

void Spi::disable_all() {
	GPIO_CLR = 0;
	GPIO_SET = ALL_SLAVES;
	usleep(100UL);
}

int Spi::transfer(int size, unsigned char *wr, unsigned char *rd) {

	/*
	 * Fast version -- no pause between bytes
	 */

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

	//std::cout << "before ioctl len=" << msg.len <<  std::endl;

	int r = ioctl(spi_cs_fd, SPI_IOC_MESSAGE(1), &msg);

	//std::cout << "after ioctl len=" << msg.len << " r=" << r << std::endl;

	if(r == -1) {
		perror("couldnt send spi msg");
		exit(1);
	}

	return 0;
}

