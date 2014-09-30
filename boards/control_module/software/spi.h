
#ifndef __SPI_H__
#define __SPI_H__

extern int spi_cs_fd;

int spi_setup(void);
int spi_readwrite(size_t size, unsigned char *wr, unsigned char *rd);

#endif // __SPI_H__

