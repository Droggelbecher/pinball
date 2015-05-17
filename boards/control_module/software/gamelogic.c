
#include <stdio.h>

#include "switches.h"
#include "solenoids.h"

void gamelogic_compute(void) {

	// Have flipper switches directly control the flipper state

	uint8_t fl_left = switches_get(SPI_SWITCHES_FLIPPER_LEFT_IDX);
	uint8_t fl_right = switches_get(SPI_SWITCHES_FLIPPER_RIGHT_IDX);

	if(fl_left) { printf("LEFT "); }
	if(fl_right) { printf("RIGHT "); }
	if(fl_left || fl_right) { printf("\n"); fflush(stdout); } 

	solenoids_set(SPI_SOLENOIDS_FLIPPER_LEFT_IDX, fl_left);
	solenoids_set(SPI_SOLENOIDS_FLIPPER_RIGHT_IDX, fl_right);

	// If all targets are dropped down, move them right up

	uint8_t bank0 = switches_get_range(SPI_SWITCHES_DROP_TARGET_BANK_0_IDX, 4);
	if(bank0 == 0x0f) {
		solenoids_set(SPI_SOLENOIDS_DROP_TARGET_BANK_0_IDX, 1);
	}

	/*fflush(stdout);*/
}

