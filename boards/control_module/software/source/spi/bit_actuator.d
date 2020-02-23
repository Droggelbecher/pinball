
import std.bitmanip;
import std.datetime;

import task;

extern(C) {
	@nogc ubyte checksum(ubyte*, ubyte);
}

/**
Note:
  Translates between "active low" logic on SPI bus side
  and "active high" logic on D side.
  */
class BitActuator(Spi, Index_, int SlaveIdx): Task {

	alias Index = Index_;

	enum {
		DATA_BYTES = (Index.MAX + 7) / 8
	};

	this(Spi spi) {
		this.spi = spi;
		this.state_data[] = 0xff;
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

	override void frame_start(Duration dt) {
		state_data[DATA_BYTES] = checksum(state_data.ptr, cast(ubyte)DATA_BYTES);
		spi.transfer_and_check(cast(Spi.SlaveIndex)SlaveIdx, state_data[0 .. DATA_BYTES + 1]);
	}

	private {
		Spi spi;
		ubyte[DATA_BYTES + 1] state_data;
	}
}

