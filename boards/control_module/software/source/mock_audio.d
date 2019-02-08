
import std.datetime;

alias AudioSource = int;

void init() { }

class Playlist {
	this(string[] filenames...) {
	}

	void clear() { }

	Playlist opOpAssign(string op)(string filename) if(op == "~") {
		return this;
	}

	void play() { }
	void next() { }
	void stop() { }
	void frame_start(Duration _) { }
}

AudioSource load_sound(string filename) { return 0; }
void play(AudioSource _) { }

