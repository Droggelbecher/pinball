
import std.experimental.logger;
import switches;

class Spi {

	enum SlaveIndex {
		LEDStripe = 18,
		Lamps = 22,
		Switches = 23,
		Display = 24,
		Solenoids = 25
	};

	this() {
		warningf("Using MOCK SPI!");
	}


	void select_only(SlaveIndex slave) {
		// TODO log
	}

	void deselect_all() {
		// TODO log
	}

	void send(ubyte[] mosi) {
		// TODO log
	}

	@nogc
	const(void[]) transfer_and_check(SlaveIndex slave, void[] input) {
		// TODO log

		buffer[0 .. input.length] = 0xff;

		// Flip bit of the death star hole light barrier
		// signal as that is low-off.
		if(slave == SlaveIndex.Switches) {
			auto idx = SwitchesIndex.HOLE0;
			const uint byte_ = cast(uint)idx / 8;
			const uint bit = cast(uint)idx % 8;
			buffer[byte_] &= ~(1 << bit);
		}

		return buffer[0 .. input.length];
	}

	private:
		ubyte[40 * 8 * 16] buffer;
}

