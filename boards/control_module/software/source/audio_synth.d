
import std.math;
import std.stdio;

class Sine {

	this(double f) {
		this.f = f;
		this.phase = 0;
	}

	void fill(short[] buffer, double sample_rate) {
		if(sample_rate == 0) {
			return;
		}

		double df = 2.0 * PI * f / sample_rate;
		foreach(int i, ref buf; buffer) {
			buf = cast(short)(short.max * sin(phase + i*df));
		}
		phase += buffer.length*df;
		phase %= (2.0 * PI);
	}

private:

	double f;
	//double df;
	double phase;
}

