
import std.stdio;
import derelict.alure.alure;

class Audio {
	public:
		static this() {
			DerelictALURE.load();
		}

		this() {
			if(!alureInitDevice(cast(char*)0, cast(int*)(0))) {
				//throw new Exception("Unable to initialize Audio Device");
				perror("Unable to initialize default audio device.");
			}
		}
};

