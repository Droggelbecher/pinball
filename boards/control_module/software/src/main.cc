
#include "main.h"

#include <time.h>
#include <unistd.h>
#include <glob.h>
#include <signal.h>

#include "config.h"
#include "game_logic.h"
#include "interval_stats.h"

namespace pinball {

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

} // ns pinball

#if MOCK_SPI
	#include "game_interface/dummy_interface.h"

	namespace pinball {
		DummyInterface spi_interface;
		using SpiInterface = DummyInterface;
	}

#else
	#include "spi/spi.h"
	#include "spi/spi_interface.h"

	namespace pinball {
		Spi spi;
		SpiInterface spi_interface {
			spi,
			DISPLAY_MODULE_COUNT,
			Coordinate<>(DISPLAY_MODULE_ROWS, DISPLAY_MODULE_COLUMNS)
		};
	}
#endif


#if CURSES_UI
	#include "game_interface/curses_interface.h"

	namespace pinball {
		using Interface = CursesInterface<SpiInterface>;
		Interface interface {
			Coordinate<>(DISPLAY_MODULE_ROWS, DISPLAY_MODULE_COUNT * DISPLAY_MODULE_COLUMNS),
			spi_interface
		};
	}
#else
	namespace pinball {
		using Interface = SpiInterface;
		Interface& interface = spi_interface;
	}
#endif

namespace pinball {
	using GameLogic_t = GameLogic<Interface>;
} // ns pinball

void sig_handler(int signum) {
	exit(1);
}


int main(int argc, const char **argv) {

	using namespace pinball;

	IntervalStats<Interface::Logger> fps_reporter("FPS", 10.0, interface.logger());

	// When aborted w/ Ctrl-C, make sure to still call exit(),
	// such that gprof will generate its gmon.out
	signal(SIGINT, sig_handler);

	Framer framer(DISPLAY_TARGET_FPS);
	GameLogic_t game_logic(interface);

	fill_playlist();
	Audio::instance().playlist_play();

	while(true) {
		framer.next_frame();

		double dt = framer.get_last_frame_duration_us() / 1000000.0f;
		fps_reporter.next_frame(dt);
		fps_reporter.add_value(1.0 / dt);

		game_logic.next_frame(dt);
	}

}

