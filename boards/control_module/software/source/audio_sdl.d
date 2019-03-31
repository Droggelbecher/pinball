
import std.datetime;
import std.string;

import derelict.sdl2.sdl;
import derelict.sdl2.mixer;

import task;
import utils;

class AudioInterface {
	static this() {
		DerelictSDL2.load();
		DerelictSDL2Mixer.load();
	}

	this() {
		Mix_Init(MIX_INIT_MP3);
		if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
			throw new Exception("Mix_OpenAudio");
		}
	}

	~this() {
		Mix_Quit();
	}

	@nogc
	void frame_start(Duration _) {
	}
}

class Sound: Task {
	this(string filename, int _ = 10) {
		chunk = Mix_LoadWAV((filename ~ ".wav").toStringz);
	}

	~this() {
		Mix_FreeChunk(chunk);
	}

	void play() {
		Mix_PlayChannel(-1, chunk, 0);
	}

	@nogc override void frame_start(Duration dt) { }

	private:
		Mix_Chunk *chunk;
}

alias MultiSound = Sound;

class Playlist: Task {

	this(string[] filenames...) {
		this.filenames = filenames.dup;
		this.index = 0;
	}

	void play() {
		playing = true;
	}

	void set_volume(double v) {
		Mix_VolumeMusic(cast(int)(v * MIX_MAX_VOLUME));
	}

	@nogc void next() {
		if(music) {
			Mix_FreeMusic(music);
			music = null;
			index++;
			if(index >= filenames.length) {
				index = 0;
			}
		}
	}

	@nogc override void frame_start(Duration dt) {
		assumeNoGC(&frame_start_)(dt);
	}

	void frame_start_(Duration dt) {
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
}

