
#include <stdio.h>
#include "audio.h"

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

	alGenSources(1, &audio_source_music_);
	alSourcei(audio_source_music_, AL_LOOPING, 1);
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

void audio_music_play() {
	alSourcePlay(audio_source_music_);
}

void audio_music_stop() {
	alSourceStop(audio_source_music_);
}

void audio_sound_play(audio_source_t src) {
	alSourcePlay(src);
}

void audio_sound_stop(audio_source_t src) {
	alSourceStop(src);
}

void audio_music_append(const char *filename) {
	ALuint buffer_id = alureCreateBufferFromFile(filename);
	if(buffer_id == AL_NONE) {
		perror("could not create sound buffer");
	}
	/*assert(track->buffer_id != AL_NONE);*/
	alSourceQueueBuffers(audio_source_music_, 1, &buffer_id);
}

void audio_music_clear() {
	alSourceStop(audio_source_music_);
	alDeleteSources(1, &audio_source_music_);
	alGenSources(1, &audio_source_music_);
}

