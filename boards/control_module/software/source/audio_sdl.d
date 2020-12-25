
import std.datetime: Duration, msecs;
import std.string;
import std.file;
import std.path;
import std.random: uniform;

import derelict.sdl2.sdl;
import derelict.sdl2.mixer;

import task;
import utils;

private {
	bool _audio_initialized = false;
}


void initialize_audio() {
	Mix_Init(MIX_INIT_MP3);
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		throw new Exception("Mix_OpenAudio");
	}
	_audio_initialized = true;
}

static this() {
	DerelictSDL2.load();
	DerelictSDL2Mixer.load();
}

static ~this() {
	if(_audio_initialized) {
		Mix_Quit();
	}
}


/**
  Preload & play sounds (.wav files) from a directory on the filesystem.
  */
class SoundRepository {
	this(string dir) {
		if(_audio_initialized) {
			foreach(string path; dirEntries(dir, "*.wav", SpanMode.breadth)) {
				string name = baseName(stripExtension(path));
				chunks[name] = Mix_LoadWAV(path.toStringz);
				// TODO: print/log
			}
		}
	}

	~this() {
		foreach(chunk; chunks.byValue) {
			Mix_FreeChunk(chunk);
		}
		chunks.clear;
	}

	void play(string name) {
		if(!_audio_initialized) {
			return;
		}
		Mix_PlayChannel(-1, chunks[name], 0);
	}

	private:
		Mix_Chunk*[string] chunks;
}

class Playlist: Task {

	this(string[] filenames...) {
		this.filenames = filenames.dup;
		this.index = 0;
	}

	void play() {
		if(!_audio_initialized) {
			return;
		}
		this.playing = true;
	}

	void set_volume(double v) {
		if(!_audio_initialized) {
			return;
		}
		Mix_VolumeMusic(cast(int)(v * MIX_MAX_VOLUME));
	}

	void set_random(bool r) {
		if(!_audio_initialized) {
			return;
		}
		this.random = r;
	}

	void next() {
		if(!_audio_initialized) {
			return;
		}
		if(music) {
			Mix_FreeMusic(music);
			music = null;
			
			if(this.random) {
				index = cast(uint)uniform(0, this.filenames.length);
			}
			else {
				index++;
			}

			if(index >= filenames.length) {
				index = 0;
			}
		}
	}

	Duration fade_out() {
		if(!_audio_initialized) {
			return 0.msecs;
		}
		int duration = 3000;
		if(playing && music) {
			playing = false;
			Mix_FadeOutMusic(duration);
			return msecs(duration);
		}
		return 0.msecs;
	}

	override void frame_start(Duration dt) {
		if(!_audio_initialized) {
			return;
		}
		if(playing && !Mix_PlayingMusic()) {
			next();
			music = Mix_LoadMUS(filenames[index].toStringz);
			Mix_PlayMusic(music, 1);
		}
	}

	private:
		Mix_Music *music;
		string[] filenames;
		uint index = 0;
		bool playing = false;
		bool random = false;
}

