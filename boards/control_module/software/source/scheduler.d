
import std.stdio: writeln;
import task : Task;
import core.thread: Thread;
import std.datetime;
import std.datetime.stopwatch;
import std.conv;
import core.stdc.stdio;
import std.string;
import std.experimental.logger;

import interval: Interval;


class Scheduler {
	enum target_fps = 30.0;
	enum target_duration = usecs(cast(long)(1_000_000 / target_fps));

	this() {
		this.stopping = false;
	}

	void add(Task task) {
		tasks ~= task;
	}

	void stop() {
		stopping = true;
	}

	void run() {
		static log_interval = Interval!Duration(5000.msecs);

		std.datetime.stopwatch.StopWatch sw;
		Duration frame_duration = target_duration;

		sw.start();
		while(!stopping) {
			frame_start(frame_duration);
			if(stopping) { break; }

			if(log_interval(frame_duration)) {
				infof("fps: %f\n", 1_000_000.0 / frame_duration.total!"usecs");
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
		// I. Run all frame_start() methods.
		//    Those are run unconditionally, before any run() methods
		//    and are expected to execute swiftly and can not yield.
		foreach(task; tasks) {
			task.frame_start(dt);
			if(task.quitting()) {
				stop();
				return;
			}
		}

		// II. Run run() for those tasks that have their run conditions met
		foreach(task; tasks) {
			task.check_run();
		}
		
		// III. Check any other requests from tasks, eg. scheduling of new tasks
		Task[] to_schedule;
		foreach(task; tasks) {
			to_schedule ~= task.pop_schedule_requests();
		}
		tasks ~= to_schedule;
	}

	private:
		Task[] tasks;

		bool stopping;

}

