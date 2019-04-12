
/**
  Convenience struct for the common pattern of
  counting something (eg loop iterations) and doing something whenever that counter increased
  by at least
  `threshold` since the last run.

  Example report file loading every 10000 bytes:
  {{{
  auto file_tracker = Interval!uint(10000);
  while(...) {
  	auto bytes_read = read_chunk_from_file(...);
  	if(file_tracker(bytes_read)) {
  		writefln("Read %d bytes", file_tracker.total);
  	}
  }
  }}}
**/

struct Interval(T) {
	this(T threshold, T zero = T()) {
		this.threshold = threshold;
		this.aggregate = zero;
		this.total = zero;
		this.zero = zero;
		this.calls = 0;
		this.reset = false;
	}

	bool opCall(T v) {
		if(reset) {
			aggregate = zero;
			calls = 0;
			reset = false;
		}

		aggregate += v;
		total += v;
		calls++;
		if(aggregate >= threshold) {
			reset = true;
			return true;
		}
		return false;
	}

	@property
	auto mean() {
		return aggregate / calls;
	}

	public:
		T total;

	private:
		T threshold;
		T aggregate;
		T zero;
		uint calls;
		bool reset;
}

