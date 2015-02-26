
#include "gpio.h"
#include "spi.h"
#include "display.h"
#include "render.h"

int main(int argc, char **argv) {
	gpio_setup();
	spi_setup();
	display_setup();

	printf("v2\n");

	int g = 1;
	while(1) {
		display_start_frame();

		/*display_render_gradient();*/
		/*display_render_frame();*/
		/*display_render_selftest_fill();*/
		render_selftest_colorstripes();
		
		/*display_render_fill(g ? 0xff : 0, g ? 0 : 0xff);*/
		g = !g;

		display_refresh();
		display_end_frame();
		display_debug_fps();
	}

	return 0;
}


