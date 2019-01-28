
class Spi {

	enum SlaveIndex {
		Lamps = 22,
		Switches = 23,
		Display = 24,
		Solenoids = 25
	};

	@nogc
	const(void[]) transfer_and_check(SlaveIndex slave, void[] input) {
		buffer[0 .. input.length] = cast(size_t[])input;
		return buffer[0 .. input.length];
	}

	private:
		size_t[40 * 8 * 16 / size_t.sizeof] buffer;
}

