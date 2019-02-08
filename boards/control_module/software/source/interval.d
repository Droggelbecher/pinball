

struct Interval(T) {
	this(T threshold, T zero = T()) {
		this.threshold = threshold;
		this.aggregate = zero;
		this.zero = zero;
	}

	bool opCall(T v) {
		aggregate += v;
		if(aggregate >= threshold) {
			aggregate = zero;
			return true;
		}
		return false;
	}

	private:
		T threshold;
		T aggregate;
		T zero;
}

