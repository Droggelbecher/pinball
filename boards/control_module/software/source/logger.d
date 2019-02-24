
import std.format;
import std.stdio;

interface Logger {
	void log(string line);
}

class WritelnLogger: Logger {
	void log(string line) {
		writeln(line);
	}
}

void logf(Args...)(Logger logger, string fmt, Args args) {
	logger.log(format(fmt, args));
}

