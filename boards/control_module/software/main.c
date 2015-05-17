
#include "display.h"
#include "gamelogic.h"
#include "gpio.h"
#include "pcf.h"
#include "render.h"
#include "solenoids.h"
#include "spi.h"
#include "spi_ss.h"
#include "switches.h"
#include "scroll.h"

int main(int argc, char **argv) {
	gpio_setup();
	spi_setup();
	display_setup();

	gpio_set_output(SPI_SS_PIN_LAMPS);
	gpio_set_output(SPI_SS_PIN_SWITCHES);
	gpio_set_output(SPI_SS_PIN_DISPLAY);
	gpio_set_output(SPI_SS_PIN_SOLENOIDS);

	struct pcf_font font;
	pcf_read_font("gohufont-11.pcf", &font);
	pcf_print_font(&font);

	struct scroll_state marquee;

	marquee.start_column = DISPLAY_MODULE_COUNT * DISPLAY_MODULE_COLUMNS;
	marquee.start_row = 0;
	marquee.speed_columns = -10;
	marquee.speed_rows = 0;
	scroll_reset(&marquee);

	while(1) {
		display_start_frame();

		switches_read();

		gamelogic_compute();

		solenoids_write();

		render_selftest_colorstripes();
		/*render_clear();*/

		scroll_compute(&marquee);
		uint8_t r = pcf_render_string(&font, "Hello, World!", marquee.current_row, marquee.current_column, 4);
		if(marquee.current_column < 0 && !r) {
			scroll_reset(&marquee);
		}

		display_refresh();

		display_end_frame();
		display_debug_fps();
	}

	return 0;
}


