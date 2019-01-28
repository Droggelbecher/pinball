

class BitSensor(Spi, Index_, int SlaveIdx) {
	import std.bitmanip;
	import std.datetime;

	alias Index = Index_;

	enum {
		DATA_WORDS = (Index.MAX + size_t.sizeof - 1) / size_t.sizeof,
		DATA_BYTES = (Index.MAX + 7) / 8
	};

	this(Spi spi) {
		this.spi = spi;
		this.state = BitArray(state_data, Index.MAX);
		this.state_data[] = cast(size_t)0;
	}

	alias state this;

	@nogc
	bool opIndex(Index idx) {
		return state[idx];
	}

	@nogc
	bool opIndexAssign(bool v, Index idx) {
		return state[idx] = v;
	}

	@nogc
	void frame_start(Duration dt) {
		const(void[]) answer = spi.transfer_and_check(cast(Spi.SlaveIndex)SlaveIdx, state_data[0 .. DATA_WORDS]);
		if(answer.length == DATA_WORDS) {
			this.state_data[0 .. DATA_WORDS] = cast(const size_t[])answer;
		}
	}

	private {
		Spi spi;
		size_t[DATA_WORDS + 1] state_data;
		BitArray state;
	}

}

