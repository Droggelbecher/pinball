
import std.stdio: writeln;


class Scheduler {
	import task : Task;
	import core.thread: Thread;
	import std.datetime: seconds, msecs;

	Task[] tasks; // = new Task[];

	void run() {
		foreach(i; 0 .. 100) {
			//writeln("frame ", i);
			frame_start(0.1);
			Thread.sleep(100.msecs);
		}
	}

	void frame_start(double dt) {
		foreach(task; tasks) {
			task.frame_start(dt);
		}

		foreach(task; tasks) {
			task.check_run();
		}
	}

}

