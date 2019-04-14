
#ifndef __SPI_SS_H__
#define __SPI_SS_H__

#include "gpio.h"

namespace pinball {

void spi_ss_activate_only(int n);
void spi_ss_deactivate_all(void);

} // ns pinball

#endif // __SPI_SS_H__

