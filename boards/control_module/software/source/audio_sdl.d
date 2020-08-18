
import std.datetime: Duration, msecs;
import std.string;
import std.file;
import std.path;
import std.random: uniform;

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

}

/**
  Preload & play sounds (.wav files) from a directory on the filesystem.
  */
class SoundRepository {
	this(string dir) {
		foreach(string path; dirEntries(dir, "*.wav", SpanMode.breadth)) {
			string name = baseName(stripExtension(path));
			chunks[name] = Mix_LoadWAV(path.toStringz);
		}
	}

	~this() {
		foreach(chunk; chunks.byValue) {
			Mix_FreeChunk(chunk);
		}
		chunks.clear;
	}

	void play(string name) {
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
		this.playing = true;
	}

	void set_volume(double v) {
		Mix_VolumeMusic(cast(int)(v * MIX_MAX_VOLUME));
	}

	void set_random(bool r) {
		this.random = r;
	}

	void next() {
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
		int duration = 3000;
		if(playing && music) {
			playing = false;
			Mix_FadeOutMusic(duration);
			return msecs(duration);
		}
		return 0.msecs;
	}

	override void frame_start(Duration dt) {
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

