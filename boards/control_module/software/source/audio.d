
import std.stdio;
import std.string;
import std.datetime;

import derelict.alure.alure;

import utils: assumeNoGC;
import logger: Logger, logf;

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
		enum MUSIC_CHUNK_SIZE = (512 * 1024);

		this(Logger logger, string[] filenames...) {
			alGenSources(1, &source);
			alSourcef(source, AL_GAIN, 0.7);

			this.logger = logger;
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
			logger.logf("Playing: %s", filenames[index]);
			stream = alureCreateStreamFromFile(
					cast(char*)filenames[index].toStringz,
					MUSIC_CHUNK_SIZE, MUSIC_BUFFERS,
					buffers.ptr
			);
			alurePlaySourceStream(source, stream, MUSIC_BUFFERS, 0, &eos_callback, cast(void*)this);
			alure_update();
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

		@nogc
		void frame_start(Duration _) {
			assumeNoGC(&alure_update)();
		}

	private:
		void alure_update() { alureUpdate(); }

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
		ALuint[MUSIC_BUFFERS] buffers;
		Logger logger;
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


