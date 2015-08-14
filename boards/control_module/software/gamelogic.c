
#include <stdio.h>
#include <stdint.h>

#include "gamelogic.h"

#include "audio.h"
#include "display.h"
#include "gpio.h"
#include "pcf.h"
#include "render.h"
#include "scroll.h"
#include "solenoids.h"
#include "solenoids.h"
#include "spi.h"
#include "spi_ss.h"
#include "switches.h"
#include "switches.h"
#include "timer.h"
#include <utils.h>

void gamelogic_compute(void) {

	static struct timer_state lamp_timer = { 0 };
	static int current_lamp = 0;
	static struct edgerising_state fl_left_rise = { 0 };
	static struct edgerising_state fl_right_rise = { 0 };
	
	// display text
	
	struct pcf_font font;
	pcf_read_font("gohufont-11.pcf", &font);
	static struct scroll_state marquee = { -20, 0, DISPLAY_MODULE_COUNT * DISPLAY_MODULE_COLUMNS, 0 };
	static char disp_text[100];




	// Have flipper switches directly control the flipper state

	uint8_t fl_left = switches_get(SPI_SWITCHES_FLIPPER_LEFT_IDX);
	uint8_t fl_right = switches_get(SPI_SWITCHES_FLIPPER_RIGHT_IDX);

	solenoids_set(SPI_SOLENOIDS_FLIPPER_LEFT_IDX, fl_left);
	solenoids_set(SPI_SOLENOIDS_FLIPPER_RIGHT_IDX, fl_right);


	// also, flippers control lamps

	if(edgerising_compute(&fl_left_rise, fl_left)) {
		printf("LEFT\n");
		current_lamp--;
	}

	if(edgerising_compute(&fl_right_rise, fl_right)) {
		printf("RIGHT\n");
		current_lamp++;
	}

	current_lamp = limit(current_lamp, 0, 64);
	lamps_clear();
	lamps_set(current_lamp, 1);
	lamps_write();


	// display text

	render_clear();
	snprintf(disp_text, sizeof(disp_text) - 1, "lamp %d", current_lamp);
	scroll_compute(&marquee);
	uint8_t r = pcf_render_string(&font, disp_text, marquee.current_row, marquee.current_column, 2);
	if(marquee.current_column < 0 && !r) {
		scroll_reset(&marquee);
	}


	// If all targets are dropped down, move them right up

	uint8_t bank0 = switches_get_range(SPI_SWITCHES_DROP_TARGET_BANK_0_IDX, 5);
	/*printf("%02x\n", bank0);*/
	if(bank0 == 0x1f) {
		printf("BANK0\n");
		fflush(stdout);
		solenoids_set(SPI_SOLENOIDS_DROP_TARGET_BANK_0_IDX, 1);
	}
	else {
		solenoids_set(SPI_SOLENOIDS_DROP_TARGET_BANK_0_IDX, 0);
	}


}

