
#ifndef MOCK_AUDIO_H
#define MOCK_AUDIO_H


class Audio {

	public:
		typedef int audio_source_t;

		static Audio& instance() {
			static Audio instance_;
			return instance_;
		}

		void playlist_append(const char *filename) { };
		void playlist_clear() { };
		void playlist_play() { };
		void playlist_stop() { };

		audio_source_t sound_load(const char *filename) { };
		void sound_play(audio_source_t) { };
		void sound_stop(audio_source_t) { };

		void update() { };

	private:

		Audio() { };
};


#endif // MOCK_AUDIO_H

