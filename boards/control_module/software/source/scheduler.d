
import std.stdio: writeln;


class Scheduler {
	import task : Task;
	import core.thread: Thread;
	import std.datetime;
	import std.conv;
	import core.stdc.stdio;
	import std.string;

	enum target_fps = 30.0;
	enum target_duration = usecs(cast(long)(1_000_000 / target_fps));

	Task[] tasks;

	this() {
		stopping = false;
	}

	void stop() {
		stopping = true;
	}

	void run() {
		StopWatch sw;
		Duration frame_duration = target_duration;

		sw.start();
		while(!stopping) {
			frame_start(frame_duration);
			debug(print_scheduler_fps) {
				fprintf(stderr, "fps: %lf dur=%s tgt=%s\n",
						1_000_000.0 / frame_duration.total!"usecs",
						frame_duration.toString.toStringz,
						target_duration.toString.toStringz
				);
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

	bool stopping;

}

