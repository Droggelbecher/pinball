
class Spi {

	enum SlaveIndex {
		Lamps = 22,
		Switches = 23,
		Display = 24,
		Solenoids = 25
	};

	size_t[] transfer_and_check(SlaveIndex slave, size_t[] input) {
		return input.dup();
	}
}

