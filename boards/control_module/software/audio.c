
#include <stdio.h>
#include "audio.h"

void audio_setup(void) {
#if AUDIO_DEBUG
	ALCsizei count;
	int i;
	const ALCchar** device_names = alureGetDeviceNames(1, &count);
	for(i = 0; i < count; i++) {
		printf("Found audio device: %s\n", device_names[i]);
	}
#endif

	if(!alureInitDevice(0, 0)) {
		perror("Couldnt init default audio device.");
	}
}

void audio_load(struct audio_track *track, const char *filename) {
	alGenSources(1, &(track->source));
	if(alGetError() != AL_NO_ERROR) {
		perror("Failed to create OpenAL source.");
	}

	track->buffer_id = alureCreateBufferFromFile(filename);
	/*assert(track->buffer_id != AL_NONE);*/

	// Set source parameters
	alSourcei(track->source, AL_BUFFER, track->buffer_id);
}

void audio_play(struct audio_track *track) { //, audio_callback_t callback) {
	alSourcePlay(track->source);
	/*alurePlaySource(track->source, callback, (void*)track);*/
}

void audio_stop(struct audio_track *track) {
	alSourceStop(track->source);
}

