
#include "gpio.h"
#include "spi.h"
#include "display.h"

int main(int argc, char **argv) {
	gpio_setup();
	spi_setup();
	display_setup();

	while(1) {
		display_start_frame();
		display_render_gradient();
		/*display_render_frame();*/
		display_refresh();
		display_end_frame();

		display_debug_fps();
	}

	return 0;
}


