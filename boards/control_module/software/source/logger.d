
import std.format;

interface Logger {
	void log(string line);
}

void logf(Args...)(Logger logger, string fmt, Args args) {
	logger.log(format(fmt, args));
}

