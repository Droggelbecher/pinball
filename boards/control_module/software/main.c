
#include "gpio.h"
#include "spi.h"
#include "display.h"
#include "render.h"
#include "buttons.h"
#include "solenoids.h"

int main(int argc, char **argv) {
	gpio_setup();
	spi_setup();
	display_setup();

	gpio_set_output(SPI_SS_PIN_LAMPS);
	gpio_set_output(SPI_SS_PIN_BUTTONS);
	gpio_set_output(SPI_SS_PIN_DISPLAY);
	gpio_set_output(SPI_SS_PIN_SOLENOIDS);

	while(1) {
		display_start_frame();

		buttons_read();

		gamelogic_compute();

		solenoids_write();

		render_selftest_colorstripes();
		display_refresh();

		display_end_frame();
		display_debug_fps();
	}

	return 0;
}


