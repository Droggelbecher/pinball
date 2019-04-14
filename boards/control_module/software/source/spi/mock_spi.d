
class Spi {

	enum SlaveIndex {
		LEDStripe = 18,
		Lamps = 22,
		Switches = 23,
		Display = 24,
		Solenoids = 25
	};

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

		buffer[0 .. input.length] = cast(ubyte[])input;
		return buffer[0 .. input.length];
	}

	private:
		ubyte[40 * 8 * 16] buffer;
}

