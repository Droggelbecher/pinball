
#ifndef AUDIO_H
#define AUDIO_H

#include <stdint.h>
#include <AL/alure.h>

#define AUDIO_DEBUG 1

typedef ALuint audio_handle_t;

struct audio_track {
	ALuint source;
	ALuint buffer_id;
	size_t buffer_size;
	const ALubyte *buffer;
};

//typedef (*audio_callback_t)(void*, ALuint);

void audio_setup(void);

void audio_load(struct audio_track*, const char*);
void audio_play(struct audio_track *track);
void audio_stop(struct audio_track *track);


#endif // AUDIO_H

