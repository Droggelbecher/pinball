
#ifndef AUDIO_H
#define AUDIO_H

#include <cstdint>
#include <vector>
#include <string>

#include <alure.h>

class Audio {
		static const int MUSIC_BUFFERS = 2;
		static const int MUSIC_CHUNK_SIZE = (512 * 1024);

	public:
		typedef ALuint audio_source_t;

		static Audio& instance() {
			static Audio instance_;
			return instance_;
		}

		void playlist_append(const char *filename);
		void playlist_clear();
		void playlist_play();
		void playlist_stop();

		audio_source_t sound_load(const char *filename);
		void sound_play(audio_source_t);
		void sound_stop(audio_source_t);

		void update();

	private:

		Audio();

		static void eos_callback(void *userdata, ALuint source);

		audio_source_t music_source_ = 0;
		alureStream *music_stream_ = 0;
		ALuint music_buffers_[MUSIC_BUFFERS];
		std::size_t playlist_index_ = 0;
		std::vector<std::string> playlist_;
};


#endif // AUDIO_H

