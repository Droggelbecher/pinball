
import std.stdio;
import std.string;
import std.datetime;

import derelict.alure.alure;

alias ALuint AudioSource;

void init() {
	DerelictALURE.load();
	DerelictAL.load();
	if(!alureInitDevice(cast(char*)0, cast(int*)(0))) {
		perror("Unable to initialize default audio device.");
	}
}


class Playlist {
	public:
		enum MUSIC_BUFFERS = 3;

		this(string[] filenames...) {
			alGenSources(1, &source);
			alureStreamSizeIsMicroSec(AL_TRUE);
			alSourcef(source, AL_GAIN, 0.7);
			this.filenames = filenames.dup;
		}

		void clear() {
			filenames.length = 0;
		}

		Playlist opOpAssign(string op)(string filename) if(op == "~") {
			filenames ~= filename;
			return this;
		}

		void play() {
			stream = alureCreateStreamFromFile(
					cast(char*)filenames[index].toStringz,
					1000000, // microseconds
					0, null
			);
			alurePlaySourceStream(source, stream, MUSIC_BUFFERS, 0, &eos_callback, cast(void*)this);
		}

		void next() {
			index++;
			if(index >= filenames.length) {
				index = 0;
			}
		}
			

		void stop() {
			alSourceStop(source);
			if(stream) {
				alureDestroyStream(stream, 0, null);
				stream = null;
			}
		}

		void frame_start(Duration _) {
			alureUpdate();
		}

	private:
		extern(C) static void eos_callback(void *userdata, ALuint source) {
			Playlist instance = cast(Playlist)userdata;

			with(instance) {
				stop;
				next;
				play;
			}
		}

		uint index = 0;
		AudioSource source = 0;
		string[] filenames;
		alureStream *stream = null;
};

AudioSource load_sound(string filename) {
	AudioSource src;
	ALuint buffer_id;
	alGenSources(1, &src);
	if(alGetError() != AL_NO_ERROR) {
		perror("Failed to create OpenAL source.");
	}
	buffer_id = alureCreateBufferFromFile(cast(char*)filename.toStringz);

	// Set source parameters
	alSourcei(src, AL_BUFFER, buffer_id);
	// Set sound volume to 100%
	alSourcef(src, AL_GAIN, 1.0);
	return src;
}

alias play = alSourcePlay;


