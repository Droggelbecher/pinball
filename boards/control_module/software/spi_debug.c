
#include <stdint.h>

#include "gpio.h"
#include "spi.h"
#include "display.h"

int main(int argc, char **argv) {
	gpio_setup();
	spi_setup();
	display_setup();

	printf("sizeof(unsigned)=%d\n", sizeof(unsigned));

	/*char line[1024];*/

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

			/*case 'g':*/
				/*puts("rendering gradient\n");*/
				/*render_gradient();*/
				/*break;*/

			/*case 'f':*/
				/*puts("rendering frame\n");*/
				/*render_frame();*/
				/*break;*/

			case 'C':
				puts("clearing screen\n");
				render_clear();
				break;

			case 'R':
				puts("rendering red\n");
				render_fill(0xff, 0x00);
				break;

			case 'G':
				puts("rendering green\n");
				render_fill(0x00, 0xff);
				break;

			case 'Y':
				puts("rendering yellow\n");
				render_fill(0x80, 0xff);
				break;


			case '9':
				puts("activating display SS\n");
				spi_ss_activate_only(5);
				printf("GPIO_CLR=%x\n", (unsigned)GPIO_CLR);
				printf("GPIO_SET=%x\n", (unsigned)GPIO_SET);
				printf("GPIO_GET=%x\n", (unsigned)GPIO_GET);
				break;

			/*case 'c':*/
				/*GPIO_CLR = 0;*/
				/*printf("GPIO_CLR=%x\n", (unsigned)GPIO_CLR & SPI_SS_SLAVES_MASK);*/
				/*printf("GPIO_SET=%x\n", (unsigned)GPIO_SET & SPI_SS_SLAVES_MASK);*/
				/*printf("GPIO_GET=%x\n", (unsigned)GPIO_GET & SPI_SS_SLAVES_MASK);*/
				/*break;*/

			/*case 'C':*/
				/*GPIO_CLR = SPI_SS_SLAVES_MASK;*/
				/*printf("GPIO_CLR=%x\n", (unsigned)GPIO_CLR & SPI_SS_SLAVES_MASK);*/
				/*printf("GPIO_SET=%x\n", (unsigned)GPIO_SET & SPI_SS_SLAVES_MASK);*/
				/*printf("GPIO_GET=%x\n", (unsigned)GPIO_GET & SPI_SS_SLAVES_MASK);*/
				/*break;*/

			/*case 's':*/
				/*GPIO_SET = 0;*/
				/*printf("GPIO_CLR=%x\n", (unsigned)GPIO_CLR & SPI_SS_SLAVES_MASK);*/
				/*printf("GPIO_SET=%x\n", (unsigned)GPIO_SET & SPI_SS_SLAVES_MASK);*/
				/*printf("GPIO_GET=%x\n", (unsigned)GPIO_GET & SPI_SS_SLAVES_MASK);*/
				/*break;*/

			/*case 'S':*/
				/*GPIO_SET = SPI_SS_SLAVES_MASK;*/
				/*printf("GPIO_CLR=%x\n", (unsigned)GPIO_CLR & SPI_SS_SLAVES_MASK);*/
				/*printf("GPIO_SET=%x\n", (unsigned)GPIO_SET & SPI_SS_SLAVES_MASK);*/
				/*printf("GPIO_GET=%x\n", (unsigned)GPIO_GET & SPI_SS_SLAVES_MASK);*/
				/*break;*/

			case '2':
				puts("activating buttons SS\n");
				spi_ss_activate_only(SPI_SS_PIN_BUTTONS);
				break;

			case 'b': {
					while(1) {
						puts("activating buttons SS\n");
						spi_ss_activate_only(SPI_SS_PIN_BUTTONS);

						puts("polling buttons\n");

						uint8_t request[] = { 0, 1, 2, 3, 4, 5, 6, 7, 0 };
						uint8_t answer[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

						spi_readwrite(9, request, answer);
						printf("%02x %02x %02x %02x %02x\n", answer[0], answer[1], answer[2], answer[3], answer[4]);
						printf("%02x %02x %02x %02x\n", answer[5], answer[6], answer[7], answer[8]);
						fflush(stdout);
						/*usleep(100UL);*/

						puts("deactivating all SS\n");
						spi_ss_deactivate_all();

						usleep(1000000UL);
					}
				}
				break;

				
			case '1':
				puts("activating display SS\n");
				GPIO_CLR = SPI_SS_SLAVES_MASK;
				GPIO_SET = SPI_SS_SLAVES_MASK;
				GPIO_CLR = (1 << SPI_SS_PIN_DISPLAY);
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

			case 'm':
				puts("writing module 0 to SPI\n");
				spi_readwrite(DISPLAY_MODULE_COLUMNS * DISPLAY_MODULE_ROWS * DISPLAY_MODULE_COLORS, display_screen_, 0);
				break;
		}

	}

	return 0;
}

