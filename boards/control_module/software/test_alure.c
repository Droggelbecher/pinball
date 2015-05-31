
#include <glob.h>
#include "audio.h"

void fill_playlist(void);


void fill_playlist(void) {
	glob_t pglob;

	int r = glob("resources/music/*.mp3", 0, 0, &pglob);
	if(r != 0) {
		perror("glob error on looking for music tracks.");
	}

	int i;
	for(i = 0; i < 3; i++) {
		audio_music_append(pglob.gl_pathv[i]);
	}

	globfree(&pglob);
}

int main(int argc, char **argv) {
	audio_setup();
	fill_playlist();

	audio_music_play();

	while(1) ;

	return 0;
}

