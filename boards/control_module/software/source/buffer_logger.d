
import std.datetime.systime;
import std.datetime;
import std.format;

import logger;

class BufferLogger(int N) : Logger {

	void log(string line) {
		auto time = Clock.currTime;
		append(
			format!"%02d:%02d:%02d  %s"(time.hour, time.minute, time.second, line)
		);
	}

	int opApply(int delegate(string) dg) {
		int r = 0;
		for(int i = begin; i != end; i = (i + 1) % N) {
			r = dg(lines[i]);
			if(r) { break; }
		}
		return r;
	}

	bool empty() {
		return (begin == end);
	}

	string front() {
		return lines[begin];
	}

	void popFront() {
		if(!empty) {
			begin = (begin + 1) % N;
		}
	}

private:

	void append(string line) {
		lines[end] = line;
		end = (end + 1) % N;
		if(begin == end) {
			begin = (begin + 1) % N;
		}
	}

	int begin = 0;
	int end = 0;
	string[N] lines;
}

