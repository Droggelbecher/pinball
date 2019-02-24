
import std.stdio;
import std.conv;

import derelict.alure.alure;

import utils: assumeNoGC;
import logger: Logger, logf;

struct AudioInterface {
	public:

		static this() {
			DerelictAL.load();
		}

		this(Logger logger) {
			this.logger = logger;

			ALCdevice* device = alcOpenDevice(null);
			if(device is null) {
				perror("Unable to initialize default audio device.");
			}

			context = alcCreateContext(device, null);
			alcMakeContextCurrent(context);
			checkAL("setting context");

			ALCsizei n;
			ALCchar** devnames = alureGetDeviceNames(false, &n);
			for(int i=0; i<n; i++) {
				logger.logf("Found audio device: %s", to!string(devnames[i]));
			}
		}

		/+
		@nogc
		void frame_start(Duration _) {
			assumeNoGC(&al_update)();
		}
		+/

	private:
		ALCcontext* context = null;
		Logger logger = null;

		void checkAL(string action) {
			auto err = alGetError();
			if(logger !is null && err != AL_NO_ERROR) {
				logger.logf("OpenAL Error during %s: %d", action, err);
			}
		}
}

package:

import mad;
import core.sys.posix.sys.mman;
import std.file;
import std.stdio;
//import core.sys.posix.sys.stat;
//import std.mmfile;

struct Sound {
	this(string filename) {
		stream = new mad_stream;
		mad_stream_init(stream);

		synth = new mad_synth;
		mad_synth_init(synth);

		frame = new mad_frame;
		mad_frame_init(frame);

		fill_buffer();
	}

	private:

		void load_file(string filename) {
			auto size = getSize(filename);
			auto file = File(filename, "r");
			ubyte* map = cast(ubyte*)mmap(null, size, PROT_READ, MAP_SHARED, file.fileno, 0);
			mad_stream_buffer(stream, map, size);
		}

		void fill_buffer() {
			// TODO: insert some kind of loop here
			auto err = mad_frame_decode(frame, stream);
			if(err) {
				//if(MAD_RECOVERABLE(mad_stream.error) || (mad_stream.error == MAD_ERROR_BUFLEN)) {
					//continue;
				//}
				//else {
					//break;
				//}
			}
			mad_synth_frame(synth, frame);

			writefln("Channels: %d Samplerate: %d", frame.header.mode + 1, frame.header.samplerate);
			// TODO: give mad_frame.header & mad_synth.pcm to OpenAL
		} // fill_buffer()

		mad_stream stream;
		mad_synth synth;
		mad_frame frame;
}


