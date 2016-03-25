
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

int main(int argc, const char **argv) {
	GameLogic game_logic;

	fill_playlist();

	Audio::instance().playlist_play();

	while(true) {
		game_logic.next_frame();

		usleep(10UL * 1000UL); // nanoseconds!
	}

}

