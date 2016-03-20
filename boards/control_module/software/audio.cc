
#include "audio.h"

#include <cstdio> // perror

Audio::Audio() {
	if(!alureInitDevice(0, 0)) {
		perror("Couldnt init default audio device.");
	}

	alGenSources(1, &music_source_);
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

void Audio::sound_play(audio_source_t source) {
	alSourcePlay(source);
}

void Audio::sound_stop(audio_source_t source) {
	alSourceStop(source);
}
