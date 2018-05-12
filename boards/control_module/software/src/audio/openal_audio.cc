
#include "audio/openal_audio.h"

#include <cstdio> // perror

namespace pinball {

Audio::Audio() {
	if(!alureInitDevice(0, 0)) {
		perror("Couldnt init default audio device.");
	}

	alGenSources(1, &music_source_);

	// Set music volume to 70%
	alSourcef(music_source_, AL_GAIN, 0.7);
}

void Audio::playlist_append(const char *filename) {
	playlist_.push_back(filename);
}

void Audio::playlist_clear() {
	playlist_.clear();
}

void Audio::playlist_play() {
	music_stream_ = alureCreateStreamFromFile(playlist_[0].c_str(), MUSIC_CHUNK_SIZE, MUSIC_BUFFERS, music_buffers_); 
	alurePlaySourceStream(music_source_, music_stream_, MUSIC_BUFFERS, 0, &eos_callback, (void*)0);
}

void Audio::playlist_stop() {
	alSourceStop(music_source_);
}


void Audio::eos_callback(void *userdata, ALuint source) {
	Audio &instance_ = Audio::instance();

	instance_.playlist_index_++;

	// start playlist from beginning
	if(instance_.playlist_index_ > instance_.playlist_.size()) {
		instance_.playlist_index_ = 0;
	}

	const char *filename = instance_.playlist_[instance_.playlist_index_].c_str();
	alureDestroyStream(instance_.music_stream_, MUSIC_BUFFERS, instance_.music_buffers_);
	instance_.music_stream_ = alureCreateStreamFromFile(filename, MUSIC_CHUNK_SIZE, MUSIC_BUFFERS, instance_.music_buffers_);
	alurePlaySourceStream(instance_.music_source_, instance_.music_stream_, MUSIC_BUFFERS, 0, &Audio::eos_callback, (void*)0);
}

void Audio::update() {
	alureUpdate();
}

Audio::audio_source_t Audio::sound_load(const char *filename) {
	audio_source_t src;
	ALuint buffer_id;

	alGenSources(1, &src);
	if(alGetError() != AL_NO_ERROR) {
		perror("Failed to create OpenAL source.");
	}

	buffer_id = alureCreateBufferFromFile(filename);

	// Set source parameters
	alSourcei(src, AL_BUFFER, buffer_id);

	// Set sound volume to 100%
	alSourcef(src, AL_GAIN, 1.0);
	return src;
}


void Audio::sound_play(audio_source_t source) {
	alSourcePlay(source);
}

void Audio::sound_stop(audio_source_t source) {
	alSourceStop(source);
}

} // ns pinball

