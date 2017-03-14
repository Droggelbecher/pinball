
#include "main.h"

#include <time.h>
#include <unistd.h>
#include <glob.h>

#include "game_logic.h"


void fill_playlist(void) {
	glob_t pglob;

	int r = glob("resources/music/*.mp3", 0, 0, &pglob);
	if(r != 0) {
		perror("glob error on looking for music tracks.");
	}

	unsigned i;
	for(i = 0; i < pglob.gl_pathc; i++) {
		Audio::instance().playlist_append(pglob.gl_pathv[i]);
	}

	globfree(&pglob);
}

	#if MOCK_SPI
		#include "interface/curses_interface.h"
		CursesInterface interface { Coordinate<>(16, 3 * 8) };
		using GameLogic_t = GameLogic<
			CursesInterface //, curses_interface
			>;
	#else
		#include "interface/spi_interface.h"
		#include "spi/linux_spi.h"
		#include "display/spi_display.h"
		#include "switches.h"
		#include "solenoids.h"

		Spi spi;
		//SpiDisplay display { spi, 3, Coordinate<>(16, 8) };
		//Switches switches { spi };
		//Solenoids solenoids { spi };
		SpiInterface interface { spi, 3, Coordinate<>(16, 8) };
		using GameLogic_t = GameLogic<
			SpiInterface //, spi_interface
			>;
	#endif

int main(int argc, const char **argv) {


	GameLogic_t game_logic(interface);

	fill_playlist();

	Audio::instance().playlist_play();

	while(true) {
		game_logic.next_frame();

		//usleep(10UL * 1000UL); // nanoseconds!
	}

}

