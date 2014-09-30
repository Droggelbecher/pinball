
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>

#include <linux/spi/spidev.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TARGET_FPS 50.0

#define SPI_MODE SPI_MODE_0
#define SPI_BITS_PER_WORD 8
#define SPI_BUS_SPEED 1000000
//#define SPI_BUS_SPEED 4000000

int spi_cs_fd;

//
// <GPIO>
//

// http://elinux.org/RPi_Low-level_peripherals

#define BCM2708_PERI_BASE        0x20000000
#define GPIO_BASE                (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)

int  mem_fd;
void *gpio_map;

// I/O access
volatile unsigned *gpio;


// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))

#define GPIO_SET *(gpio+7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR *(gpio+10) // clears bits which are 1 ignores bits which are 0

#define GET_GPIO(g) (*(gpio+13)&(1<<g)) // 0 if LOW, (1<<g) if HIGH


//
// </GPIO>
//

#define ROWS 16
#define COLUMNS 8
#define COLORS 2
#define RED 0
#define GREEN 1


enum { SS_BTNS = 23, SS_DISP = 24, SS_SOL = 25, SS_LMPS = 22 };

unsigned char V[] = { 0b00000000, 0b00000001, 0b00010001, 0b00100101,
  0b01010101, 0b01110101, 0b11011011, 0b11111011, 0b11111111 };

unsigned char screen[COLORS][ROWS][COLUMNS];



void render_stuff() {
	static int phase = 0;
	int row, col;
	for(row = 0; row < ROWS; row++) {
	for(col = 0; col < COLUMNS; col++) {
//		screen[RED][(row + phase/64) % 16][col] = V[ ((row % 8)) % 8];
//		screen[GREEN][(row + phase/64) % 16][col] = V[ ((col % 8)) % 8];
		screen[RED][(row + phase/64) % 16][col] = V[ ((row % 8 + (int)(phase*16/TARGET_FPS))) % 8];
		screen[GREEN][(row + phase/64) % 16][col] = V[ ((col % 8 + (int)(phase*32/TARGET_FPS))) % 8];
	}
	}

	phase++;
	phase %= 8 * 64;
}


int open_spi() {
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

	usleep(1000);

	return 0;
}

void slave_select(int n) {
	// http://raspberrypiguide.de/howtos/raspberry-pi-gpio-how-to/ 

	GPIO_SET = (1 << SS_BTNS) | (1 << SS_DISP) | (1 << SS_SOL) | (1 << SS_LMPS);
	GPIO_CLR = (1 << n);
	usleep(1000);
}

void slave_deselect() {
	GPIO_SET = (1 << SS_BTNS) | (1 << SS_DISP) | (1 << SS_SOL) | (1 << SS_LMPS);
	GPIO_CLR = 0;
	usleep(1000);
}
	

void setup_gpio() {
   /* open /dev/mem */
   if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
      printf("can't open /dev/mem \n");
      exit(-1);
   }

   /* mmap GPIO */
   gpio_map = mmap(
      NULL,             //Any adddress in our space will do
      BLOCK_SIZE,       //Map length
      PROT_READ|PROT_WRITE,// Enable reading & writting to mapped memory
      MAP_SHARED,       //Shared with other processes
      mem_fd,           //File to map
      GPIO_BASE         //Offset to GPIO peripheral
   );

   close(mem_fd); //No need to keep mem_fd open after mmap

   if (gpio_map == MAP_FAILED) {
      printf("mmap error %d\n", (int)gpio_map);//errno also set!
      exit(-1);
   }

   // Always use volatile pointer!
   gpio = (volatile unsigned *)gpio_map;


//	INP_GPIO(22); OUT_GPIO(22);
//	INP_GPIO(23); OUT_GPIO(23);
//	INP_GPIO(24); OUT_GPIO(24);

	int g;
	int rep;

	  // Set GPIO pins 7-11 to output
	  for (g=22; g<=25; g++)
	  {
	    INP_GPIO(g); // must use INP_GPIO before we can use OUT_GPIO
	    OUT_GPIO(g);
	  }

/*
	  for (rep=0; rep<10; rep++)
	  {
	     for (g=22; g<=25; g++)
	     {
printf("%d\n", g);
		slave_select(g);
//		GPIO_CLR = 1 << g;
//		GPIO_SET = 0xffffffff;
	
	     //  GPIO_SET = 1<<(g);

		sleep(5);
	     }
	  }
*/
}

double get_time() {
	struct timeval  tv;
	gettimeofday(&tv, NULL);

	double time_in_mill = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
	return time_in_mill;
}
   

int main(int argc, char **argv) {
	setup_gpio();
	

	unsigned char send[] = { 'x', 'y', 'z', '0' };
	//unsigned char send[] = { 0xaa, 0x55, 0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80, 0xaa, 0x55, 0xaa };
	//unsigned char send[] = { 0x00 , 0xff, 0xaa, 0x55 }; // 0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80, 0xaa, 0x55 };
//	unsigned char send[] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
	//unsigned char send[] = { 0x08, 0x08, 0x08, 0x08 };
	open_spi();
	unsigned char recv[1024];


	int frame = 0;
	while(1) {
		double loop_start = get_time();

		render_stuff();

		slave_select(SS_DISP);
		spi_readwrite(COLORS * ROWS * COLUMNS, (char*)screen, 0);
		slave_deselect();

//		usleep( 20000 ); // 20ms = 50 fps (little slower due to rendering and comm.)

		frame++;

		usleep( 1000.0 * ((1000.0 / TARGET_FPS) - (get_time() - loop_start)) );

		if(frame == 100) {
			double loop_duration = get_time() - loop_start;
			printf("%lf fps\n",  1000.0 / (double)loop_duration);
			frame = 0;
		}

	}

	//while(1) ;

	return 0;
}




