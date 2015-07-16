
#include <stdio.h>

#include "switches.h"
#include "solenoids.h"
#include "timer.h"

void gamelogic_compute(void) {

	static struct timer_state lamp_timer = { 0 };
	static int current_lamp = 0;


	// Have flipper switches directly control the flipper state

	uint8_t fl_left = switches_get(SPI_SWITCHES_FLIPPER_LEFT_IDX);
	uint8_t fl_right = switches_get(SPI_SWITCHES_FLIPPER_RIGHT_IDX);

	if(fl_left) { printf("LEFT "); }
	if(fl_right) { printf("RIGHT "); }
	if(fl_left || fl_right) { printf("\n"); fflush(stdout); } 

	solenoids_set(SPI_SOLENOIDS_FLIPPER_LEFT_IDX, fl_left);
	solenoids_set(SPI_SOLENOIDS_FLIPPER_RIGHT_IDX, fl_right);

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



	// Lamps

	/*printf("frame %ld\n", (long)display_frame);*/
	/*fflush(stdout);*/
	
	if(timer_fired(&lamp_timer)) {
		timer_set(&lamp_timer, 1.1);
		lamps_clear();

		/*printf("lamp=%d\n", current_lamp);*/
		/*fflush(stdout);*/

		lamps_set(current_lamp, 1);
		lamps_write();

		++current_lamp;
		if(current_lamp > 64) {
			current_lamp = 0;
		}
	}

}

