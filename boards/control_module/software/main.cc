
#include "main.h"

#include <time.h>
#include <unistd.h>

#include "game_logic.h"

int main(int argc, char **argv) {

	GameLogic game_logic;

	while(true) {
		game_logic.next_frame();

		usleep(10UL * 1000UL); // nanoseconds!
	}

}

