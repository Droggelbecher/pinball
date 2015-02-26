
#ifndef __DEBUG_GPIO_H__
#define __DEBUG_GPIO_H__

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define GPIO_SET_INPUT(g) 
#define GPIO_SET_OUTPUT(g) 

#define GPIO_SET _gpio_dummy
#define GPIO_CLR _gpio_dummy
#define GPIO_GET _gpio_dummy

void gpio_setup(void);

extern unsigned long _gpio_dummy;

#endif // __DEBUG_GPIO_H__
