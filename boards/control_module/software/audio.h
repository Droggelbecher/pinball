
#ifndef AUDIO_H
#define AUDIO_H

#include <stdint.h>
#include <AL/alure.h>

#define AUDIO_DEBUG 1

typedef ALuint audio_source_t;

audio_source_t audio_source_music_;

void audio_setup(void);

void audio_music_append(const char *filename);
void audio_music_clear(void);
void audio_music_play(void);
void audio_music_stop(void);

audio_source_t audio_sound_load(const char *filename);
void audio_sound_play(audio_source_t);
void audio_sound_stop(audio_source_t);

#endif // AUDIO_H

