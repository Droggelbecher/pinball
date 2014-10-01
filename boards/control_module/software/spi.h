
#ifndef __SPI_H__
#define __SPI_H__

#include <fcntl.h>
#include <linux/spi/spidev.h>
#include <linux/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <time.h>
#include <unistd.h>

extern int spi_cs_fd;

int spi_setup(void);
int spi_readwrite(size_t size, unsigned char *wr, unsigned char *rd);

#endif // __SPI_H__

