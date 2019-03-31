
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
//import logger: Logger, logf;
import task: Task;
import std.experimental.logger;

private {
	alias mad_fixed_t = int;
}


class AudioInterface {
	public:

		static this() {
			DerelictAL.load();
		}

		this() {
			ALCdevice* device = alcOpenDevice(null);
			if(device is null) {
				check_al("open audio device");
			}

			infof("Using audio device: %s", to!string(alcGetString(device, ALC_DEVICE_SPECIFIER)));

			context = alcCreateContext(device, null);
			alcMakeContextCurrent(context);
			check_al("setting context");

		}

		@nogc
		void frame_start(Duration _) {
		}

	private:
		ALCcontext* context = null;

}

class Playlist : Sound {
	public:
		this(string[] filenames...) {
			this.filenames = filenames.dup;
			super(this.filenames[0]);
		}

		void clear() {
			filenames.length = 0;
		}

		Playlist opOpAssign(string op)(string filename) if(op == "~") {
			filenames ~= filename;
			return this;
		}

		override void play() {
			infof("Playlist playing: %s", filenames[index]);
			super.play();
		}

		void next() {
			index++;
			if(index >= filenames.length) {
				index = 0;
			}
		}

		override void rewind() {
			next();
			load_file(filenames[index]);
			play();
		}

	private:
		Sound sound = null;
		uint index = 0;
		string[] filenames;
}


class MultiSound: Task {
	this(string filename, int n = 4) {
		sounds ~= new Sound(filename);
		for(int i = 1; i < n; i++) {
			sounds ~= new Sound(sounds[0].map, sounds[0].size);
		}
	}

	void set_volume(double v) {
		foreach(sound; sounds) {
			sound.set_volume(v);
		}
	}

	int state() {
		foreach(sound; sounds) {
			if(sound.state() == AL_PLAYING) {
				return AL_PLAYING;
			}
		}
		return sounds[0].state();
	}

	void play() {
		foreach(sound; sounds) {
			if(sound.state() != AL_PLAYING) {
				sound.play();
				break;
			}
		}
	}

	void stop() {
		foreach(sound; sounds) {
			sound.stop();
		}
	}

	@nogc
	override void frame_start(Duration dt) {
		foreach(sound; sounds) {
			sound.frame_start(dt);
		}
	}
	
	private:
		Sound[] sounds;
}


class Sound: Task {

	enum BUFFERS = 4;
	enum BUFFER_SIZE = 16 * 1024;

	this(string filename) {
		this.filename = filename;

		stream = new mad_stream; mad_stream_init(stream);
		synth = new mad_synth;   mad_synth_init(synth);
		frame = new mad_frame;   mad_frame_init(frame);

		alGenSources(1, &source);
		check_al("gen_sources");
		alSourcef(source, AL_GAIN, 1.0);

		alGenBuffers(buffers.length, buffers.ptr);

		load_file(filename);
	}

	this(ubyte* map, ulong size) {
		this.map = map;
		this.size = size;

		stream = new mad_stream; mad_stream_init(stream);
		synth = new mad_synth;   mad_synth_init(synth);
		frame = new mad_frame;   mad_frame_init(frame);

		alGenSources(1, &source);
		check_al("gen_sources");
		alSourcef(source, AL_GAIN, 1.0);

		mad_stream_buffer(stream, map, cast(uint)size);

		alGenBuffers(buffers.length, buffers.ptr);
		prefill_buffers();
	}


	~this() {
		mad_frame_finish(frame);
		//mad_synth_finish(synth);
		mad_stream_finish(stream);
	}

	int state() {
		int state;
		alGetSourcei(source, AL_SOURCE_STATE, &state);
		return state;
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

	void log_frame(Duration dt) {
		tracef("frame_start dt=%d", dt.total!"msecs");
	}

	@nogc
	override void frame_start(Duration dt) {
		assumeNoGC(&log_frame)(dt);

		int state;
		alGetSourcei(source, AL_SOURCE_STATE, &state);
		if(state == AL_STOPPED) {
			//logger.logf("%s has stopped playing", this.filename);
			assumeNoGC(&rewind)();
		}

		update_buffers();
	}

	void rewind() {
		alSourceRewind(source);
		mad_stream_buffer(stream, map, cast(uint)size);
		prefill_buffers();
	}

	protected:
		void load_file(string filename) {
			// Stop sound, unqueue all queued buffers
			alSourceRewind(source);
			clear_all_buffers();

			// unmap audio file
			if(map) {
				munmap(map, cast(uint)size);
				map = null;
			}

			// Now load new one
			this.size = getSize(filename);
			auto file = File(filename, "r");

			tracef("%s (%d bytes)", filename, this.size);
			/*
				Note on resource management:
				- There will be a limited number of sound files being used throughout the whole game
				- Thus no point in unmapping (unless for the playlist case)
				- For MultiSound unmapping will be dangerous as pointer to mapped area is shared!
			*/
			this.map = cast(ubyte*)mmap(null, cast(uint)size, PROT_READ, MAP_SHARED, file.fileno, 0);
			if(map == MAP_FAILED) {
				throw new Exception("mmap failed");
			}
			mad_stream_buffer(stream, map, cast(uint)size);
			prefill_buffers();
		}

	private:
		/*
		Resources:
		http://m.baert.free.fr/contrib/docs/libmad/doxy/html/low-level.html
		LibMAD & LibAO: http://home.eeworld.com.cn/my/space-uid-179477-blogid-32464.html
		LibMAD & OpenAL: https://github.com/monokrome/openal-source/blob/master/src/openal_mp3sample.cpp
		*/

		void clear_all_buffers() {
			// Precondition: Source is in stopped or initial state
			// prefill is only called when there iss completely new audio data to be played

			ALint processed;
			ALint queued;
			alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);
			alGetSourcei(source, AL_BUFFERS_QUEUED, &queued);

			// Unqueue processed buffers
			alSourceUnqueueBuffers(source, processed, null);
			assumeNoGC(&check_al)("unqueue");

			// Clear all pending buffers
			alSourcei(source, AL_BUFFER, AL_NONE);
			assumeNoGC(&check_al)("clear pending");

			alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);
			alGetSourcei(source, AL_BUFFERS_QUEUED, &queued);

			// Source should now be completely buffer-less
			assert(processed == 0);
			assert(queued == 0);
		}

		void prefill_buffers() {
			clear_all_buffers();

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
				// thus no point in queueing them.
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
					break;
				}
				alSourceQueueBuffers(source, 1, &buffer);
				assumeNoGC(&check_al)("queue");
			}
		}

		bool fill_buffer(ALuint buffer_id) {
			short[BUFFER_SIZE / 2] buffer = void;
			short[] to_fill = buffer;
			int channels = void;

			tracef("fill_buffer %d", buffer_id);

			do {
				tracef("mad_frame_decode to_fill=%d", to_fill.length);
				int r = mad_frame_decode(frame, stream);
				if(r != 0) {
					if(stream.error == mad_error.MAD_ERROR_BUFLEN) {
						//break;
						continue;
					}
					else {
						check_mad(stream.error, "frame_decode");
						warningf("MAD: %s", mad_errorstring[stream.error]);
					}
				}
				
				mad_synth_frame(synth, frame);
				channels = (frame.header.mode == mad_mode.MAD_MODE_SINGLE_CHANNEL) ? 1 : 2;
				size_t n = min(synth.pcm.length, to_fill.length / channels);
				if(channels == 1) {
					for(int i=0; i<n; i++) {
						to_fill[i] = synth.pcm.samples[0][i]
							.clip
							.mad_decode!short;
					}
				}
				else { // Stereo / Dual-channel
					for(int i=0; i<n; i++) {
						to_fill[i * 2] = synth.pcm.samples[0][i]
							.clip .mad_decode!short;
						to_fill[i * 2 + 1] = synth.pcm.samples[1][i]
							.clip .mad_decode!short;
					}
				}
				
				to_fill = to_fill[n * channels..$];
				
			} while(to_fill.length >= synth.pcm.length * channels);

			tracef("buflen=%d to_fill=%d", buffer.length, to_fill.length);
			int sz  = cast(int)(buffer.length - to_fill.length) * 2;
			auto samplerate = frame.header.samplerate;
			alBufferData(buffer_id,
				(frame.header.mode == mad_mode.MAD_MODE_SINGLE_CHANNEL)
					? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16,
				cast(void*)buffer.ptr, sz, samplerate
			);
			check_al("alBufferData");

			return sz != 0;
		} // fill_buffer

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
		//static const ulong scalebits = MAD_F_FRACBITS + 1 - T.sizeof * 8;
		enum scalebits = 28 + 1 - T.sizeof * 8;
		return cast(T)(sample >> scalebits);
	}

	mad_fixed_t clip(mad_fixed_t sample) {
		enum {
			MIN = -0x10000000L,
			MAX =  0x10000000L - 1
		}
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

			// round (by adding 1 to highest bit of B)
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


