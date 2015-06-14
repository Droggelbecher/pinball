
#include <stdio.h>
#include "audio.h"

audio_source_t audio_music_source_ = 0;
alureStream *audio_music_stream_ = 0;
char *audio_music_playlist_[] = {
	"resources/music/cantina_band_noize_tank_remix.mp3",

	"resources/music/binary_suns_coyote_kisses_remix.mp3",
	"resources/music/imperial_march_brostep_remix.mp3",
	"resources/music/imperial_march_heavy_metal_remix.mp3",
	"resources/music/jedi_theme_electro_remix.mp3",
	"resources/music/star_wars_launchpad_freestyle_remix.mp3",
	"resources/music/star_wars_techno_trance_remix.mp3"
};
int audio_music_playlist_index_ = 0;
int audio_music_playlist_length_ = sizeof(audio_music_playlist_);
ALuint audio_music_buffers_[AUDIO_MUSIC_BUFFERS];


void audio_setup(void) {
#if AUDIO_DEBUG
	ALCsizei count;
	int i;
	const ALCchar** device_names = alureGetDeviceNames(1, &count);
	puts("Audio devices found:\n");
	for(i = 0; i < count; i++) {
		printf("  %s\n", device_names[i]);
	}
#endif

	if(!alureInitDevice(0, 0)) {
		perror("Couldnt init default audio device.");
	}

	alGenSources(1, &audio_music_source_);
	/*alSourcei(audio_source_music_, AL_LOOPING, 1);*/


}

void audio_music_play() {
	/*alSourcePlay(audio_source_music_);*/

	audio_music_stream_ = alureCreateStreamFromFile(audio_music_playlist_[0], AUDIO_MUSIC_CHUNK_SIZE, AUDIO_MUSIC_BUFFERS, audio_music_buffers_); 
	alurePlaySourceStream(audio_music_source_, audio_music_stream_, AUDIO_MUSIC_BUFFERS, 0, &audio_eos_callback_, (void*)0);
}

void audio_eos_callback_(void *userdata, ALuint source) {

	printf("audio_eos_callback\n");
	fflush(stdout);

	audio_music_playlist_index_++;
	if(audio_music_playlist_index_ > audio_music_playlist_length_) {
		audio_music_playlist_index_ = 0;
	}


	const char *filename = audio_music_playlist_[audio_music_playlist_index_];
	printf("idx=%d filename=%s\n",  audio_music_playlist_index_, filename);
	fflush(stdout);
	
	alureDestroyStream(audio_music_stream_, AUDIO_MUSIC_BUFFERS, audio_music_buffers_);
	audio_music_stream_ = alureCreateStreamFromFile(filename, AUDIO_MUSIC_CHUNK_SIZE, AUDIO_MUSIC_BUFFERS, audio_music_buffers_); 
	alurePlaySourceStream(audio_music_source_, audio_music_stream_, AUDIO_MUSIC_BUFFERS, 0, &audio_eos_callback_, (void*)0);
} 

void audio_update() {
	alureUpdate();
}


audio_source_t audio_sound_load(const char *filename) {
	audio_source_t src;
	ALuint buffer_id;

	alGenSources(1, &src);
	if(alGetError() != AL_NO_ERROR) {
		perror("Failed to create OpenAL source.");
	}

	buffer_id = alureCreateBufferFromFile(filename);

	// Set source parameters
	alSourcei(src, AL_BUFFER, buffer_id);
	return src;
}

void audio_music_stop() {
	alSourceStop(audio_music_source_);
}

void audio_sound_play(audio_source_t src) {
	alSourcePlay(src);
}

void audio_sound_stop(audio_source_t src) {
	alSourceStop(src);
}

void audio_music_append(const char *filename) {
	/*ALuint buffer_id = alureCreateBufferFromFile(filename);*/
	/*if(buffer_id == AL_NONE) {*/
		/*perror("could not create sound buffer");*/
	/*}*/
	/*[>assert(track->buffer_id != AL_NONE);<]*/
	/*alSourceQueueBuffers(audio_source_music_, 1, &buffer_id);*/
}

void audio_music_clear() {
	/*alSourceStop(audio_source_music_);*/
	/*alDeleteSources(1, &audio_source_music_);*/
	/*alGenSources(1, &audio_source_music_);*/
}

