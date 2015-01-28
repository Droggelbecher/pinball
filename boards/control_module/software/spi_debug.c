
#include "gpio.h"
#include "spi.h"
#include "display.h"

int main(int argc, char **argv) {
	gpio_setup();
	spi_setup();
	display_setup();

	printf("sizeof(unsigned)=%d\n", sizeof(unsigned));

	char line[1024];

	gpio_set_output(SPI_SS_PIN_LAMPS);
	gpio_set_output(SPI_SS_PIN_BUTTONS);
	gpio_set_output(SPI_SS_PIN_DISPLAY);
	gpio_set_output(SPI_SS_PIN_SOLENOIDS);

	while(1) {
		printf("> ");
		fflush(stdout);
		int c = getc(stdin);
		getc(stdin); // newline
		/*puts("\n");*/
		if(c == EOF) { break; }

		switch(c) {
			case 'r':
				puts("refreshing display\n");
				display_refresh();
				break;

			case 'g':
				puts("rendering gradient\n");
				display_render_gradient();
				break;

			case 'f':
				puts("rendering frame\n");
				display_render_frame();
				break;

			case '9':
				puts("activating display SS\n");
				spi_ss_activate_only(5);
				printf("GPIO_CLR=%x\n", (unsigned)GPIO_CLR);
				printf("GPIO_SET=%x\n", (unsigned)GPIO_SET);
				printf("GPIO_GET=%x\n", (unsigned)GPIO_GET);
				break;

			case 'c':
				GPIO_CLR = 0;
				printf("GPIO_CLR=%x\n", (unsigned)GPIO_CLR & SPI_SS_SLAVES_MASK);
				printf("GPIO_SET=%x\n", (unsigned)GPIO_SET & SPI_SS_SLAVES_MASK);
				printf("GPIO_GET=%x\n", (unsigned)GPIO_GET & SPI_SS_SLAVES_MASK);
				break;

			case 'C':
				GPIO_CLR = SPI_SS_SLAVES_MASK;
				printf("GPIO_CLR=%x\n", (unsigned)GPIO_CLR & SPI_SS_SLAVES_MASK);
				printf("GPIO_SET=%x\n", (unsigned)GPIO_SET & SPI_SS_SLAVES_MASK);
				printf("GPIO_GET=%x\n", (unsigned)GPIO_GET & SPI_SS_SLAVES_MASK);
				break;

			case 's':
				GPIO_SET = 0;
				printf("GPIO_CLR=%x\n", (unsigned)GPIO_CLR & SPI_SS_SLAVES_MASK);
				printf("GPIO_SET=%x\n", (unsigned)GPIO_SET & SPI_SS_SLAVES_MASK);
				printf("GPIO_GET=%x\n", (unsigned)GPIO_GET & SPI_SS_SLAVES_MASK);
				break;

			case 'S':
				GPIO_SET = SPI_SS_SLAVES_MASK;
				printf("GPIO_CLR=%x\n", (unsigned)GPIO_CLR & SPI_SS_SLAVES_MASK);
				printf("GPIO_SET=%x\n", (unsigned)GPIO_SET & SPI_SS_SLAVES_MASK);
				printf("GPIO_GET=%x\n", (unsigned)GPIO_GET & SPI_SS_SLAVES_MASK);
				break;

			case '2':
				puts("activating display SS\n");
				GPIO_CLR = SPI_SS_SLAVES_MASK;
				GPIO_SET = SPI_SS_SLAVES_MASK;
				GPIO_CLR = (1 << 24);
				/*spi_ss_activate_only(SPI_SS_PIN_DISPLAY);*/
				printf("GPIO_CLR=%x\n", (unsigned)GPIO_CLR);
				printf("GPIO_SET=%x\n", (unsigned)GPIO_SET);
				printf("GPIO_GET=%x\n", (unsigned)GPIO_GET);
				break;

			case '1':
				puts("activating display SS\n");
				GPIO_CLR = SPI_SS_SLAVES_MASK;
				GPIO_SET = 0;
				GPIO_CLR = SPI_SS_SLAVES_MASK;
				/*spi_ss_activate_only(SPI_SS_PIN_DISPLAY);*/
				printf("GPIO_CLR=%x\n", (unsigned)GPIO_CLR);
				printf("GPIO_SET=%x\n", (unsigned)GPIO_SET);
				printf("GPIO_GET=%x\n", (unsigned)GPIO_GET);
				break;

			case '0':
				puts("deactivating all SS\n");

				GPIO_CLR = 0;
				GPIO_SET = SPI_SS_SLAVES_MASK;
				
				/*spi_ss_deactivate_all();*/
				printf("GPIO_CLR=%x\n", (unsigned)GPIO_CLR);
				printf("GPIO_SET=%x\n", (unsigned)GPIO_SET);
				printf("GPIO_GET=%x\n", (unsigned)GPIO_GET);
				break;

			case 'd':
				puts("writing display to SPI\n");
				spi_readwrite(sizeof(display_screen_), display_screen_, 0);
				break;
		}

	}

	return 0;
}

