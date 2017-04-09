
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
	#include "game_interface/dummy_interface.h"
	#include "game_interface/curses_interface.h"
	DummyInterface dummy_interface;
	CursesInterface<DummyInterface> interface { Coordinate<>(16, 5 * 8), dummy_interface  };
	using GameLogic_t = GameLogic<
		CursesInterface<DummyInterface>
		>;
#else
	#include "interface/spi_interface.h"
	#include "spi/linux_spi.h"
	#include "display/spi_display.h"
	#include "switches.h"
	#include "solenoids.h"
	#include "game_interface/curses_interface.h"

	Spi spi;
	SpiInterface spi_interface { spi, 3, Coordinate<>(16, 8) };
	CursesInterface<SpiInterface> interface { Coordinate<>(16, 5 * 8), spi_interface };
	using GameLogic_t = GameLogic<
		CursesInterface<SpiInterface>
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

