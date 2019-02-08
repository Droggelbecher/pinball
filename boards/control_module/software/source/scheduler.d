
import std.stdio: writeln;

class Scheduler {
	import task : Task;
	import core.thread: Thread;
	import std.datetime;
	import std.conv;
	import core.stdc.stdio;
	import std.string;
	import logger: Logger, logf;
	import interval: Interval;

	enum target_fps = 30.0;
	enum target_duration = usecs(cast(long)(1_000_000 / target_fps));

	Task[] tasks;
	Logger logger;

	this(Logger logger = null) {
		this.stopping = false;
		this.logger = logger;
	}

	void stop() {
		stopping = true;
	}

	void run() {
		static log_interval = Interval!Duration(5000.msecs);

		StopWatch sw;
		Duration frame_duration = target_duration;

		sw.start();
		while(!stopping) {
			frame_start(frame_duration);

			if(logger && log_interval(frame_duration)) {
				logger.logf("fps: %f\n", 1_000_000.0 / frame_duration.total!"usecs");
			}

			Duration computation_time = sw.peek().to!Duration;
			Duration delta = target_duration - computation_time;

			if(delta >= 1.msecs) {
				Thread.sleep(delta);
			}

			// Start timing next frame (include loop overhead)
			frame_duration = sw.peek().to!Duration;
			if(sw.running) { sw.stop(); }
			sw.reset();
			sw.start();
		}
	}

	void frame_start(Duration dt) {
		foreach(task; tasks) {
			task.frame_start(dt);
		}

		foreach(task; tasks) {
			task.check_run();
		}
	}

	private:
		bool stopping;

}

