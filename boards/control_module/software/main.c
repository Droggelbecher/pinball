
#include "display.h"
#include "gamelogic.h"
#include "gpio.h"
#include "render.h"
#include "solenoids.h"
#include "spi.h"
#include "spi_ss.h"
#include "switches.h"

int main(int argc, char **argv) {
	gpio_setup();
	spi_setup();
	display_setup();

	gpio_set_output(SPI_SS_PIN_LAMPS);
	gpio_set_output(SPI_SS_PIN_SWITCHES);
	gpio_set_output(SPI_SS_PIN_DISPLAY);
	gpio_set_output(SPI_SS_PIN_SOLENOIDS);

	while(1) {
		display_start_frame();

		switches_read();

		gamelogic_compute();

		solenoids_write();

		render_selftest_colorstripes();
		display_refresh();

		display_end_frame();
		display_debug_fps();
	}

	return 0;
}


