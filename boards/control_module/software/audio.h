
#ifndef AUDIO_H
#define AUDIO_H

#include <stdint.h>
#include <AL/alure.h>

#define AUDIO_DEBUG 1

#define AUDIO_MUSIC_CHUNK_SIZE (512 * 1024)
#define AUDIO_MUSIC_BUFFERS 2

typedef ALuint audio_source_t;

audio_source_t audio_music_source_;
alureStream *audio_music_stream_;
char *audio_music_playlist_[7];
int audio_music_playlist_index_;
int audio_music_playlist_length_;
ALuint audio_music_buffers_[AUDIO_MUSIC_BUFFERS];

void audio_setup(void);
void audio_eos_callback_(void *userdata, ALuint source);

void audio_music_append(const char *filename);
void audio_music_clear(void);
void audio_music_play(void);
void audio_music_stop(void);

audio_source_t audio_sound_load(const char *filename);
void audio_sound_play(audio_source_t);
void audio_sound_stop(audio_source_t);

#endif // AUDIO_H

