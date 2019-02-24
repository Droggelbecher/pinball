
import std.stdio;
import std.string;
import std.datetime;
import std.conv;

import derelict.alure.alure;

import utils: assumeNoGC;
import logger: Logger, logf;

package {
	alias ALuint AudioSource;
}

struct AudioInterface {
	public:

		static this() {
			DerelictALURE.load();
			DerelictAL.load();
		}

		this(Logger logger) {
			this.logger = logger;
			if(!alureInitDevice(cast(char*)0, cast(int*)(0))) {
				perror("Unable to initialize default audio device.");
			}

			ALCsizei n;
			ALCchar** devnames = alureGetDeviceNames(false, &n);
			for(int i=0; i<n; i++) {
				logger.logf("Audio Device: %s", to!string(devnames[i]));
			}

			context = alcGetCurrentContext();
			alureStreamSizeIsMicroSec(false);
		}

		@nogc
		void frame_start(Duration _) {
			assumeNoGC(&alure_update)();
		}

	private:
		static ALCcontext* context = null;
		Logger logger = null;

		void alure_update() {
			alcMakeContextCurrent(context);
			alureUpdate();
		}
}


class Playlist {
	public:
		//enum MUSIC_BUFFERS = 3;
		//enum MUSIC_CHUNK_SIZE = (512 * 1024);

		// On WSL with Pulse, we need this, otherwise small samples
		// such as utini.mp3 will never play and we get a "non-0 prebuf" error (?!)
		enum MUSIC_BUFFERS = 4;
		enum MUSIC_CHUNK_SIZE = (32 * 1024);

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
			logger.logf("Playlist playing: %s", filenames[index]);
			stream = alureCreateStreamFromFile(
					cast(char*)filenames[index].toStringz,
					MUSIC_CHUNK_SIZE, MUSIC_BUFFERS,
					buffers.ptr
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
		ALuint[MUSIC_BUFFERS] buffers;
		Logger logger;
};

class SoundEffect {
		// On WSL with Pulse, we need a small chunk size (eg 20k),
		// otherwise small samples
		// such as utini.mp3 will never play and we get a "non-0 prebuf" error (?!)
		enum BUFFERS = 4;
		//enum CHUNK_SIZE = (20 * 1024);

		// For blip1.mp3 we need an even smaller one?!
		enum CHUNK_SIZE = (32 * 1024);

	public:
		this(Logger logger, string filename) {
			this.filename = filename;
			this.logger = logger;
			alGenSources(1, &source);
			if(alGetError() != AL_NO_ERROR) { perror("Failed to create source."); }
			alSourcef(source, AL_GAIN, 1.0);
			this.stream = alureCreateStreamFromFile(
				cast(char*)this.filename.toStringz,
				CHUNK_SIZE, BUFFERS,
				buffers.ptr
			);
			if(alGetError() != AL_NO_ERROR) { perror("Failed to create stream."); }
		}

		~this() {
			stop();
		}

		void play() {
			logger.logf("SoundEffect playing: %s", filename);

			alurePlaySourceStream(source, stream, BUFFERS, 0, &eos_callback, cast(void*)this);
			if(alGetError() != AL_NO_ERROR) { perror("Failed to play stream."); }
		}

		void stop() {
			logger.logf("SoundEffect stopping: %s", filename);
			alSourceStop(source);
			if(stream) {
				alureDestroyStream(stream, 0, null);
				stream = null;
			}
		}

	private:
		extern(C) static void eos_callback(void *userdata, ALuint source) {
			SoundEffect instance = cast(SoundEffect)userdata;
			instance.stop();
		}

		string filename;
		alureStream *stream = null;
		AudioSource source = 0;
		ALuint[BUFFERS] buffers;
		Logger logger;
}

//SoundEffect load_sound(string filename) {
	//return new SoundEffect(filename);
//}


/*
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
*/


