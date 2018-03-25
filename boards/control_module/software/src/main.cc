
#include "main.h"

#include <time.h>
#include <unistd.h>
#include <glob.h>

#include "config.h"
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
	CursesInterface<DummyInterface> interface {
		Coordinate<>(DISPLAY_MODULE_ROWS, DISPLAY_MODULE_COUNT * DISPLAY_MODULE_COLUMNS),
		dummy_interface
	};

	using GameLogic_t = GameLogic<
		CursesInterface<DummyInterface>
		>;
#else
	#include "spi/spi.h"
	#include "spi/spi_interface.h"
	#include "game_interface/curses_interface.h"

	Spi spi;
	SpiInterface spi_interface {
		spi,
		DISPLAY_MODULE_COUNT,
		Coordinate<>(DISPLAY_MODULE_ROWS, DISPLAY_MODULE_COLUMNS)
	};
	CursesInterface<SpiInterface> interface {
		Coordinate<>(DISPLAY_MODULE_ROWS, DISPLAY_MODULE_COUNT * DISPLAY_MODULE_COLUMNS),
		spi_interface
	};
	using GameLogic_t = GameLogic<
		CursesInterface<SpiInterface>
		>;
#endif

int main(int argc, const char **argv) {

	Framer framer(DISPLAY_TARGET_FPS);
	GameLogic_t game_logic(interface);

	fill_playlist();
	Audio::instance().playlist_play();

	while(true) {
		framer.next_frame();
		game_logic.next_frame(framer.get_last_frame_duration_us() / 1000000.0f);
	}

}

