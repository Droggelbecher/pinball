
#ifndef MOCK_AUDIO_H
#define MOCK_AUDIO_H

namespace pinball {

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

		audio_source_t sound_load(const char *filename) { return 0; };
		void sound_play(audio_source_t) { };
		void sound_stop(audio_source_t) { };

		void update() { };

	private:

		Audio() { };
};

} // ns pinball

#endif // MOCK_AUDIO_H

