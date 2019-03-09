
import core.sys.posix.sys.mman;
import std.algorithm.comparison;
import std.conv;
import std.datetime;
import std.file;
import std.format;
import std.stdio;

import mad;
import derelict.alure.alure;

import audio_synth: Sine;
import utils: assumeNoGC;
import logger: Logger, logf;
import task: Task;

struct AudioInterface {
	public:

		static this() {
			DerelictAL.load();
		}

		this(Logger logger) {
			this.logger = logger;

			ALCdevice* device = alcOpenDevice(null);
			if(device is null) {
				check_al("open audio device");
			}

			context = alcCreateContext(device, null);
			alcMakeContextCurrent(context);
			check_al("setting context");

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

}

//package:


class Sound : Task {

	enum BUFFERS = 4;
	enum BUFFER_SIZE = 16 * 1024;

	Sine sine;

	this(string filename) {
		this.filename = filename;

		this.sine = new Sine(440.0);

		stream = new mad_stream; mad_stream_init(stream);
		synth = new mad_synth;   mad_synth_init(synth);
		frame = new mad_frame;   mad_frame_init(frame);
		load_file(filename);

		alGenSources(1, &source);
		check_al("gen_sources");

		alSourcef(source, AL_GAIN, 1.0);
		//check_al("gen_buffers");

		//update_buffer(0.msecs);
		//alSourceQueueBuffers(source, buffers.length, buffers.ptr);
		//check_al("queue_buffers");
		prefill_buffers();
	}

	void play() {
		//logger.logf("SoundEffect playing: %s", filename);
		//update_buffer(0.msecs);
		//alSourceQueueBuffers(source, buffers.length, buffers.ptr);
		//check_al("queue_buffers");
		//update_buffer(

		alSourcePlay(source);
		check_al("source_play");
		// TODO: Implement
	}

	@nogc
	override void frame_start(Duration dt) {
		update_buffers();
	}

	private:
		// in D, int is guaranteed to be 4 bytes, no need for ifdef trickery
		alias mad_fixed_t = int;

		void load_file(string filename) {
			auto size = getSize(filename);
			auto file = File(filename, "r");
			ubyte* map = cast(ubyte*)mmap(null, size, PROT_READ, MAP_SHARED, file.fileno, 0);
			mad_stream_buffer(stream, map, size);
			//check_mad(stream.error, "stream_buffer");
		}

		/*
		Resources:
		http://m.baert.free.fr/contrib/docs/libmad/doxy/html/low-level.html
		LibMAD & LibAO: http://home.eeworld.com.cn/my/space-uid-179477-blogid-32464.html
		LibMAD & OpenAL: https://github.com/monokrome/openal-source/blob/master/src/openal_mp3sample.cpp
		*/

		void prefill_buffers() {
			alGenBuffers(buffers.length, buffers.ptr);
			//for(int i = 0; i < BUFFERS; i++) {
			foreach(buffer; buffers) {
				assumeNoGC(&fill_buffer)(buffer);
				alSourceQueueBuffers(source, 1, &buffer);
				assumeNoGC(&check_al)("queue");
			}
		}

		@nogc
		void update_buffers() {
			int state, processed;
			alGetSourcei(source, AL_SOURCE_STATE, &state);
			alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);
			//printf("state: %x processed: %d\n", state, processed);

			/*
			if(state == AL_STOPPED) {
				return;
			}
			*/

				// TODO: add alGetError checks everywhere
			for( ; processed; processed--) {
				//printf("-\n");
				ALuint buffer;
				alSourceUnqueueBuffers(source, 1, &buffer);
				assumeNoGC(&check_al)("unqueue");
				assumeNoGC(&fill_buffer)(buffer);
				alSourceQueueBuffers(source, 1, &buffer);
				assumeNoGC(&check_al)("queue");
			}
		}

		void fill_buffer(ALuint buffer_id) {
			short[BUFFER_SIZE / 2] buffer;
			short[] to_fill = buffer;

			while(to_fill.length) {
				writefln("to_fill: %d", to_fill.length);
				if(mad_frame_decode(frame, stream) && (stream.error != mad_error.MAD_ERROR_BUFLEN)) {
					check_mad(stream.error, "frame_decode");
					if(stream.error != 0) {
						writeln("ERROR: ", mad_errorstring[stream.error]);
					}
				}
				
				if(stream.error == mad_error.MAD_ERROR_BUFLEN) {
					break;
				}

				mad_synth_frame(synth, frame);
				
				size_t n = min(synth.pcm.length, to_fill.length);
				
				writeln(frame.header.samplerate);
				sine.fill(to_fill[0 .. n], frame.header.samplerate);
				to_fill = to_fill[n..$];
			} // while

			writefln("Mode: %d Channels: %d Samplerate: %d sz: %d",
				frame.header.mode,
				synth.pcm.channels,
				frame.header.samplerate,
				cast(int)(buffer.length - to_fill.length) * 2,
			);

			alBufferData(
					buffer_id,
					AL_FORMAT_MONO16,
					buffer.ptr,
					cast(int)(buffer.length - to_fill.length) * 2,
					frame.header.samplerate
			);
			check_al("alBufferData");
			

		} // fill_buffer



		// TODO:
		// mad error checking:
		// err = mad_frame_decode(frame, stream)
		// if err && !MAD_RECOVERABLE(stream.error) { throw }

		void fill_buffer__(ALuint buffer_id) {
			/*
			   Notes on buffer format expected by openAL:
			
			   https://wiki.delphigl.com/index.php/alBufferData
			
			   PCM 16 buffer format:
			   signed 16 bit int, stereo: 16bit left, 16bit right, ...
			*/

			ubyte[BUFFER_SIZE] buffer;
			ubyte[] to_fill = buffer;
			//writefln("buf: %d %d %d %d", buffer[0], buffer[1], buffer[2], buffer[3]);
			static int xxx = 0;

			while(to_fill.length >= 4) {
				if(mad_frame_decode(frame, stream) && (stream.error != mad_error.MAD_ERROR_BUFLEN)) {
					check_mad(stream.error, "frame_decode");
					if(stream.error != 0) {
						writeln(mad_errorstring[stream.error]);
					}
				}
				
				if(stream.error == mad_error.MAD_ERROR_BUFLEN) {
					break;
					//continue;
				}
				//*/

				  //mad_fixed_t samples[2][1152];		[> PCM output samples [ch][sample] <]
				mad_synth_frame(synth, frame);
				//check_mad(stream.error, "synth_frame");

				// nsampls = synth.pcm.length
				//printf("nsamples=%d addr=%p\n", synth.pcm.length, synth.pcm.samples);
				if(frame.header.mode != mad_mode.MAD_MODE_SINGLE_CHANNEL) { // Stereo
					/+
					mad_fixed_t* left = (*synth.pcm.samples)[0].ptr;
					mad_fixed_t* right = (*synth.pcm.samples)[1].ptr;
					for(
							int i = 0;
							i < synth.pcm.length && to_fill.length >= 4;
							i++, left++, right++, to_fill = to_fill[4..$]
					) {
						to_fill[0] = (*left) & 0xff;
						to_fill[1] = (*left >> 8) & 0xff;
						to_fill[2] = (*right) & 0xff;
						to_fill[3] = (*right >> 8) & 0xff;
					}
					+/

				}
				else if(true) { // Mono
					mad_fixed_t* samples = synth.pcm.samples; //)[0].ptr;
					for(
							int i = 0;
							i < synth.pcm.length && to_fill.length >= 2;
							i++, samples++, to_fill = to_fill[2..$]
					) {
						for(int j = 0; j < (*samples) / 10000000; j++) {
							writef("*");
						}
						writeln();
						//writefln("%d %d %d %d", 
								//(*samples >>  0) & 0xff,
								//(*samples >>  8) & 0xff,
								//(*samples >> 16) & 0xff,
								//(*samples >> 24) & 0xff,
						//);
						to_fill[0] = (*samples) & 0xff;
						to_fill[1] = (*samples >> 8) & 0xff;
						//to_fill[1] = (*samples >> 16) & 0xff;
						//to_fill[0] = (*samples >> 24) & 0xff;
					}
				}
				else {
					for(
							int i = 0;
							i < synth.pcm.length && to_fill.length >= 2;
							i++, xxx += 100000000, to_fill = to_fill[2..$]
					) {
						for(int j = 0; j < (xxx) / 10000000; j++) {
							writef("*");
						}
						writeln();
						to_fill[0] = xxx & 0xff;
						to_fill[1] = (xxx >> 8) & 0xff;
						//to_fill[2] = (xxx >> 16) & 0xff;
						//to_fill[3] = (xxx >> 24) & 0xff;
					}
				}
			}

			writefln("Mode: %d Channels: %d Samplerate: %d", frame.header.mode, synth.pcm.channels, frame.header.samplerate);
			writefln("buffering %d bytes into %d", buffer.length - to_fill.length, buffer_id);
			//writefln("buf: %d %d %d %d", buffer[0], buffer[1], buffer[2], buffer[3]);

			alBufferData(
				buffer_id,
				(frame.header.mode == mad_mode.MAD_MODE_STEREO) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16,
				buffer.ptr, cast(int)(buffer.length - to_fill.length),
				frame.header.samplerate
			);
			check_al("alBufferData");

		} // fill_buffer()

		mad_stream stream;
		mad_synth synth;
		mad_frame frame;
		ALuint source;
		ALuint[BUFFERS] buffers;
		string filename;
}


private:

	string[int] openal_error;
	
	static this() {
		openal_error[     0] = "no error";
		openal_error[0xA001] = "invalid name";
		openal_error[0xA002] = "invalid enum";
		openal_error[0xA003] = "invalid value";
		openal_error[0xA004] = "invalid operation";
		openal_error[0xA005] = "out of memory";
		openal_error[0xB001] = "vendor";
		openal_error[0xB002] = "version";
		openal_error[0xB003] = "renderer";
		openal_error[0xB004] = "extensions";
		openal_error[0xC000] = "doppler factor";
		openal_error[0xC001] = "doppler velocity";
		openal_error[0xC003] = "speed of sound";
		openal_error[0xD000] = "distance model";
		openal_error[0xD001] = "inverse distance";
		openal_error[0xD002] = "inverse distance clamped";
		openal_error[0xD003] = "linear distance";
		openal_error[0xD004] = "linear distance clamped";
		openal_error[0xD005] = "exponent distance";
		openal_error[0xD006] = "exponent distance clamped";
	};

	void check_al(string action) {
		auto err = alGetError();
		if(err != AL_NO_ERROR) {
			//logger.logf("OpenAL Error during %s: %d", action, err);
			throw new Exception(format!"%s: OpenAL Error %s"(action, openal_error[err]));
		}
	}


	string[int] mad_errorstring;

	static this() {
		mad_errorstring[0x0000] = "no error";
		mad_errorstring[0x0001] = "input buffer too small (or EOF)";
		mad_errorstring[0x0002] = "invalid (null) buffer pointer";
		mad_errorstring[0x0031] = "not enough memory";
		mad_errorstring[0x0101] = "lost synchronization";
		mad_errorstring[0x0102] = "reserved header layer value";
		mad_errorstring[0x0103] = "forbidden bitrate value";
		mad_errorstring[0x0104] = "reserved sample frequency value";
		mad_errorstring[0x0105] = "reserved emphasis value";
		mad_errorstring[0x0201] = "CRC check failed";
		mad_errorstring[0x0211] = "forbidden bit allocation value";
		mad_errorstring[0x0221] = "bad scalefactor index";
		mad_errorstring[0x0222] = "bad bitrate/mode combination";
		mad_errorstring[0x0231] = "bad frame length";
		mad_errorstring[0x0232] = "bad big_values count";
		mad_errorstring[0x0233] = "reserved block_type";
		mad_errorstring[0x0234] = "bad scalefactor selection info";
		mad_errorstring[0x0235] = "bad main_data_begin pointer";
		mad_errorstring[0x0236] = "bad audio data length";
		mad_errorstring[0x0237] = "bad Huffman table select";
		mad_errorstring[0x0238] = "Huffman data overrun";
		mad_errorstring[0x0239] = "incompatible block_type for JS";
	}

	void check_mad(int err, string msg = "") {
		if(!(err & 0xff00)) {
			throw new Exception(format!"%s: MAD Error: %s"(msg, mad_errorstring[err]));
		}
	}

