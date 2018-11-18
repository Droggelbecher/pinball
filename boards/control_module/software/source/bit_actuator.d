
import std.bitmanip;

class BitActuator(Spi, Index_, int SlaveIdx) {

	alias Index = Index_;

	enum {
		DATA_WORDS = (Index.MAX + 8*size_t.sizeof - 1) / (8*size_t.sizeof),
		DATA_BYTES = (Index.MAX + 7) / 8
	};

	this(Spi spi) {
		this.spi = spi;
		this.state = BitArray(state_data, Index.MAX);
		this.state_data[] = cast(size_t)-1;
	}

	alias state this;

	bool opIndexAssign(bool v, Index idx) {
		return state[idx] = v;
	}

	void frame_start(double dt) {
		spi.transfer_and_check(cast(Spi.SlaveIndex)SlaveIdx, state_data[0 .. DATA_WORDS]);
	}

	private {
		Spi spi;
		size_t[DATA_WORDS + 1] state_data;
		BitArray state;
	}
}

