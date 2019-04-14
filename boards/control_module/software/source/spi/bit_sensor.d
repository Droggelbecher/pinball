
import task;

class BitSensor(Spi, Index_, int SlaveIdx): Task {
	import std.bitmanip;
	import std.datetime;

	alias Index = Index_;

	enum {
		//DATA_WORDS = (Index.MAX + size_t.sizeof - 1) / size_t.sizeof,
		DATA_BYTES = (Index.MAX + 7) / 8
	};

	this(Spi spi) {
		this.spi = spi;
		this.state_data[] = cast(size_t)0;
	}

	@nogc
	bool opIndex(Index idx) {
		const uint byte_ = cast(uint)idx / 8;
		const uint bit = cast(uint)idx % 8;
		return !((state_data[byte_] >> bit) & 0x01);
	}

	@nogc
	bool opIndexAssign(bool v, Index idx) {
		const ubyte byte_ = cast(ubyte)idx / 8;
		const ubyte bit = cast(ubyte)idx % 8;

		if(!v) {
			state_data[byte_] |= (1 << bit);
		}
		else {
			state_data[byte_] &= ~(1 << bit);
		}
		return this[idx];
	}

	//@nogc
	override
	void frame_start(Duration dt) {
		auto answer = spi.transfer_and_check(cast(Spi.SlaveIndex)SlaveIdx, state_data[0 .. DATA_BYTES]);
		if(answer.length == DATA_BYTES) {
			this.state_data[] = cast(ubyte[])answer;
			//tracef("SENSOR[%s] <- %s",
					//cast(Spi.SlaveIndex)SlaveIdx,
					//map!(to!string)(state_data).join(" ")
			//);
		}
	}

	private {
		Spi spi;
		ubyte[DATA_BYTES] state_data;
	}

}

