
#include <stdio.h>

#include "buttons.h"
#include "solenoids.h"

void gamelogic_compute(void) {

	printf("%d %d\n",buttons_get(BUTTONS_FLIPPER_LEFT_IDX),buttons_get(BUTTONS_FLIPPER_RIGHT_IDX));
	/*printf("%02x %02x %02x %02x ",buttons_state[0], buttons_state[1], buttons_state[2], buttons_state[3]);*/
	/*printf("%02x %02x %02x %02x %02x\n",buttons_state[4], buttons_state[5], buttons_state[6], buttons_state[7], buttons_state[8]);*/

	solenoids_set(SOLENOIDS_FLIPPER_LEFT_IDX, buttons_get(BUTTONS_FLIPPER_LEFT_IDX));
	solenoids_set(SOLENOIDS_FLIPPER_RIGHT_IDX, buttons_get(BUTTONS_FLIPPER_RIGHT_IDX));
	/*solenoids_set(SOLENOIDS_FLIPPER_LEFT_IDX, 1);*/
	/*solenoids_set(SOLENOIDS_FLIPPER_RIGHT_IDX, 0);*/

	/*printf("%02x %02x\n", solenoids_state[0], solenoids_state[1]);*/

	fflush(stdout);
}

