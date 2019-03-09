
import core.sys.posix.sys.mman;
import std.algorithm.comparison;
import std.conv;
import std.datetime;
import std.file;
import std.format;
import std.stdio;
import std.random;

import mad;
import derelict.alure.alure;

import audio_synth: Sine;
import utils: assumeNoGC;
import logger: Logger, logf;
import task: Task;

private {
	alias mad_fixed_t = int;
}


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

		@nogc
		void frame_start(Duration _) {
		}

	private:
		ALCcontext* context = null;
		Logger logger = null;

}

class Playlist : Sound {
	public:
		this(Logger logger, string[] filenames...) {
			this.filenames = filenames.dup;
			super(logger, this.filenames[0]);
		}

		void clear() {
			filenames.length = 0;
		}

		Playlist opOpAssign(string op)(string filename) if(op == "~") {
			filenames ~= filename;
			return this;
		}

		override void play() {
			logger.logf("Playlist playing: %s", filenames[index]);
			super.play();
		}

		void next() {
			index++;
			if(index >= filenames.length) {
				index = 0;
			}
			//this.filename = filenames[index];
		}

		override void rewind() {
			//alSourceStop(source);
			next();
			load_file(filenames[index]);
			play();
		}

	private:
		Sound sound = null;
		uint index = 0;
		string[] filenames;
}


class Sound : Task {

	enum BUFFERS = 4;
	enum BUFFER_SIZE = 16 * 1024;

	this(Logger logger, string filename) {
		this.logger = logger;
		this.filename = filename;

		stream = new mad_stream; mad_stream_init(stream);
		synth = new mad_synth;   mad_synth_init(synth);
		frame = new mad_frame;   mad_frame_init(frame);

		alGenSources(1, &source);
		check_al("gen_sources");
		alSourcef(source, AL_GAIN, 1.0);

		load_file(filename);
	}

	~this() {
		mad_frame_finish(frame);
		//mad_synth_finish(synth);
		mad_stream_finish(stream);
	}

	void set_volume(double v) {
		alSourcef(source, AL_GAIN, v);
	}

	void play() {
		alSourcePlay(source);
		check_al("source_play");
	}

	void stop() {
		alSourceStop(source);
		rewind();
	}

	@nogc
	override void frame_start(Duration dt) {
		int state;
		alGetSourcei(source, AL_SOURCE_STATE, &state);
		if(state == AL_STOPPED) {
			printf("---- STATE STOPPED ---- \n");
			assumeNoGC(&rewind)();
		}

		update_buffers();
	}

	void rewind() {
		logger.logf("stopped! rewinging");
		alSourceRewind(source);
		//alSourceStop(source);
		mad_stream_buffer(stream, map, size);
		prefill_buffers();
	}

	protected:
		void load_file(string filename) {
			logger.logf("Loading %s", filename);

			// Stop sound, unqueue all queued buffers
			alSourceStop(source);
			int processed;
			alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);
			for( ; processed; processed--) {
				ALuint buffer;
				alSourceUnqueueBuffers(source, 1, &buffer);
			}

			// unmap audio file
			if(map) {
				munmap(map, size);
				map = null;
			}

			// Now load new one
			this.size = getSize(filename);
			auto file = File(filename, "r");
			this.map = cast(ubyte*)mmap(null, size, PROT_READ, MAP_SHARED, file.fileno, 0);
			mad_stream_buffer(stream, map, size);
			prefill_buffers();
		}

	private:
		/*
		Resources:
		http://m.baert.free.fr/contrib/docs/libmad/doxy/html/low-level.html
		LibMAD & LibAO: http://home.eeworld.com.cn/my/space-uid-179477-blogid-32464.html
		LibMAD & OpenAL: https://github.com/monokrome/openal-source/blob/master/src/openal_mp3sample.cpp
		*/

		void prefill_buffers() {
			// TODO: We should not regenerate those on eg. rewind
			alGenBuffers(buffers.length, buffers.ptr);
			foreach(buffer; buffers) {
				if(!assumeNoGC(&fill_buffer)(buffer)) {
					break;
				}
				alSourceQueueBuffers(source, 1, &buffer);
				assumeNoGC(&check_al)("queue");
			}
		}

		@nogc
		void update_buffers() {
			int state, processed;
			alGetSourcei(source, AL_SOURCE_STATE, &state);
			if(state != AL_PLAYING) {
				// If we're not playing we will not consume buffers,
				// thusno paint in queueing them.
				// That doesn't harm (in terms of functionality) but is not nice either,
				// lets rather not.
				return;
			}

			alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);
			for( ; processed; processed--) {
				ALuint buffer;
				alSourceUnqueueBuffers(source, 1, &buffer);
				assumeNoGC(&check_al)("unqueue");
				if(!assumeNoGC(&fill_buffer)(buffer)) {
					//assumeNoGC(&rewind)();
					break;
				}
				alSourceQueueBuffers(source, 1, &buffer);
				assumeNoGC(&check_al)("queue");
			}
		}

		bool fill_buffer(ALuint buffer_id) {
			short[BUFFER_SIZE / 2] buffer;
			short[] to_fill = buffer;
			int channels;

			do {
				int r = mad_frame_decode(frame, stream);
				if(r != 0) {
					if(stream.error == mad_error.MAD_ERROR_BUFLEN) {
						// libmad neeeds more input (input buffer too small)
						break;
					}
					else {
						check_mad(stream.error, "frame_decode");
						if(stream.error != 0) {
							writeln("MAD: ", mad_errorstring[stream.error]);
						}
					}
				}
				
				mad_synth_frame(synth, frame);

				channels = (frame.header.mode == mad_mode.MAD_MODE_SINGLE_CHANNEL) ? 1 : 2;
				size_t n = min(synth.pcm.length, to_fill.length / channels);

				if(channels == 1) {
					for(int i=0; i<n; i++) {
						to_fill[i] = 
							dither_left(synth.pcm.samples[0][i])
							.clip .mad_decode!short;
					}
				}
				
				else { // Stereo / Dual-channel
					for(int i=0; i<n; i++) {
						to_fill[i * 2] =
							dither_left(synth.pcm.samples[0][i])
							.clip .mad_decode!short;
							
						to_fill[i * 2 + 1] =
							dither_right(synth.pcm.samples[1][i])
							.clip .mad_decode!short;
					}
				}
				
				to_fill = to_fill[n * channels..$];
				
			} while(to_fill.length >= synth.pcm.length * channels);

			/*
			   Notes on buffer format expected by openAL:
			
			   https://wiki.delphigl.com/index.php/alBufferData
			
			   PCM 16 buffer format:
			   signed 16 bit int, stereo: 16bit left, 16bit right, ...
			*/
			int sz  = cast(int)(buffer.length - to_fill.length) * 2;
			alBufferData(
				buffer_id,
				(frame.header.mode == mad_mode.MAD_MODE_SINGLE_CHANNEL)
					? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16,
				cast(void*)buffer.ptr, sz,
				frame.header.samplerate
			);
			check_al("alBufferData");

			return sz != 0;
		} // fill_buffer

		Logger logger;

		mad_stream stream;
		mad_synth synth;
		mad_frame frame;
		ALuint source;
		ALuint[BUFFERS] buffers;

		string filename;
		ubyte* map;
		ulong size;

		LinearDither dither_left, dither_right;
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

	T mad_decode(T)(mad_fixed_t sample) {
		ulong scalebits = MAD_F_FRACBITS + 1 - T.sizeof * 8;
		return cast(T)(sample >> scalebits);
	}

	mad_fixed_t clip(mad_fixed_t sample) {
		enum {
			MIN = -0x10000000L,
			MAX =  0x10000000L - 1
		};
		if(sample > MAX) {
			sample = MAX;
		}
		else if(sample < MIN) {
			sample = MIN;
		}
		return sample;
	}


	struct LinearDither {
		enum bits = 16;

		mad_fixed_t opCall(mad_fixed_t sample) {
			/*
			 * Fixed-point format: 0xACCCCBBB
			 * A == whole part      (sign + 3 bits)
			 * B == fractional part (28 bits)
			 * C == PCM part        (1 whole + 15 fractional bits)
			 */

			// mask = Part B
			mad_fixed_t scalebits = MAD_F_FRACBITS + 1 - bits;
			mad_fixed_t mask = cast(mad_fixed_t)((1 << scalebits) - 1);

			sample += error[0] - error[1] + error[2];
			error[2] = error[1];
			error[1] = error[0] / 2;

			// round
			mad_fixed_t output = sample + (1 << (MAD_F_FRACBITS - bits));
			
			// add noise
			mad_fixed_t r = uniform(mad_fixed_t.min, mad_fixed_t.max);
			output += (r & mask) - (prev_random & mask);
			prev_random = r;

			output = clip(output);

			// Difference between 0xACCCC after modification and (complete) original
			error[0] = sample - (output & ~mask);
			return output;
		}

		mad_fixed_t[3] error;
		mad_fixed_t prev_random;
	}


/+
short audio_linear_dither(int bits, mad_fixed_t sample, struct audio_dither *dither, struct audio_stats *stats) {
    unsigned int scalebits;
    mad_fixed_t output, mask, random;

    enum {
        MIN = -MAD_F_ONE,
        MAX =  MAD_F_ONE - 1
    };

    /* noise shape */
    sample += dither->error[0] - dither->error[1] + dither->error[2];

    dither->error[2] = dither->error[1];
    dither->error[1] = dither->error[0] / 2;

    /* bias */
    output = sample + (1L << (MAD_F_FRACBITS + 1 - bits - 1));

    scalebits = MAD_F_FRACBITS + 1 - bits;
    mask = (1L << scalebits) - 1;

    /* dither */
    random  = prng(dither->random);
    output += (random & mask) - (dither->random & mask);

    dither->random = random;

    /* clip */
    if (output >= stats->peak_sample) 
    {
        if (output > MAX) 
        {
            ++stats->clipped_samples;

            if (output - MAX > stats->peak_clipping)
                stats->peak_clipping = output - MAX;

            output = MAX;

            if (sample > MAX)
                sample = MAX;
        }

        stats->peak_sample = output;
    }
    else if (output < -stats->peak_sample) 
    {
        if (output < MIN) 
        {
            ++stats->clipped_samples;

            if (MIN - output > stats->peak_clipping)
                stats->peak_clipping = MIN - output;

            output = MIN;

            if (sample < MIN)
                sample = MIN;
        }

        stats->peak_sample = -output;
    }

    /* quantize */
    output &= ~mask;

    /* error feedback */
    dither->error[0] = sample - output;

    /* scale */
    return output >> scalebits;
}
+/


