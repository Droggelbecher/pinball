
import core.stdc.stdio;
import core.thread: Thread;
import std.conv;
import std.datetime.stopwatch;
import std.datetime;
import std.experimental.logger;
import std.stdio: writeln;
import std.string;
import std.range;
import std.algorithm.sorting;
import std.algorithm.mutation: SwapStrategy;

import task : Task;
import interval: Interval;


class Scheduler {
	enum target_fps = 30.0;
	enum target_duration = usecs(cast(long)(1_000_000 / target_fps));

	this() {
		this.stopping = false;
	}

	void add(Task task) {
		Task[] new_tasks = [ task ];
		add(new_tasks);
	}

	void add(Task task, int priority) {
		task.priority = priority;
		Task[] new_tasks = [ task ];
		add(new_tasks);
	}

	void add(Task[] new_tasks, int priority) {
		foreach(task; new_tasks) {
			task.priority = priority;
		}
		add(new_tasks);
	}

	void add(Task[] new_tasks) {
		completeSort!(pred, SwapStrategy.stable, Task[], Task[])(tasks, new_tasks);
		tasks = assumeSorted!pred(tasks.release() ~ new_tasks);
	}


	void stop() {
		stopping = true;
	}

	void run() {
		static fps_tracker = Interval!Duration(60000.msecs);

		std.datetime.stopwatch.StopWatch sw;
		Duration frame_duration = target_duration;

		sw.start();
		while(!stopping) {
			frame_start(frame_duration);
			if(stopping) { break; }

			if(fps_tracker(frame_duration)) {
				infof("fps: %f\n", 1_000_000.0 / fps_tracker.mean.total!"usecs");
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
		//Task[] to_schedule;
		foreach(task; tasks) {
			//to_schedule ~= task.pop_schedule_requests();
			foreach(sr; task.pop_schedule_requests()) {
				add(sr.task, task.priority + sr.relative_priority);
			}
		}
		//tasks ~= to_schedule;
		//add(to_schedule);
	}

	private:
		enum string pred = "a.priority < b.priority";

		SortedRange!(Task[], pred) tasks;

		bool stopping;

}

