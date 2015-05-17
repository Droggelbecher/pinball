
// Sources:
// http://elinux.org/RPi_Low-level_peripherals
// http://raspberrypiguide.de/howtos/raspberry-pi-gpio-how-to/ 

#ifndef __RASPBERRY_GPIO_H__
#define __RASPBERRY_GPIO_H__

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define BCM2708_PERI_BASE        0x20000000
#define GPIO_BASE                (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */
#define GPIO_PAGE_SIZE (4*1024)
#define GPIO_BLOCK_SIZE (4*1024)

// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define GPIO_SET_INPUT(g) *(gpio_map+((g)/10)) &= ~(7<<(((g)%10)*3))
//#define GPIO_SET_OUTPUT(g) *(gpio_map+((g)/10)) |=  (1<<(((g)%10)*3))
//#define SET_GPIO_ALT(g,a) *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))

#define GPIO_SET *(gpio_map+7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR *(gpio_map+10) // clears bits which are 1 ignores bits which are 0

#define GPIO_GET *(gpio_map+13)

//#define GET_GPIO(g) (*(gpio+13)&(1<<g)) // 0 if LOW, (1<<g) if HIGH

extern int gpio_mem_fd;
extern volatile unsigned *gpio_map;

void gpio_setup(void);
void gpio_set_output(int n);

#endif // __RASPBERRY_GPIO_H__
