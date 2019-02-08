
import core.stdc.string;
import core.stdc.errno;
import std.conv;

extern(C) {
	@nogc int open_gpio();
	@nogc int open_spi();
	@nogc void gpio_set_output(int);
	@nogc void gpio_enable_only(uint, uint);
	@nogc void gpio_disable_all(uint);
	@nogc int spi_transfer(int, int, ubyte*, ubyte*);

	@nogc ubyte checksum(ubyte*, ubyte);
}


class Spi {

	enum BUFFER_SIZE = 10 * 1024;

	enum SlaveIndex {
		Lamps = 22,
		Switches = 23,
		Display = 24,
		Solenoids = 25,
		AllMask = (1 << Lamps)
			| (1 << Display)
			| (1 << Switches)
			| (1 << Solenoids)
	}

	this() {
		open_gpio();
		with(SlaveIndex) {
			gpio_set_output(Lamps);
			gpio_set_output(Switches);
			gpio_set_output(Display);
			gpio_set_output(Solenoids);
		}
		spi_fd = open_spi();
	}

	@nogc
	const(void[]) transfer_and_check(SlaveIndex slave, void[] input)
	in {
		assert(input.length < BUFFER_SIZE - 1);
	}
	do {
		gpio_enable_only(1 << slave, SlaveIndex.AllMask);

		//ubyte[input.length] output;
		ubyte[] output = buffer[0 .. input.length];
		output[] = 0xAA;

		spi_transfer(spi_fd, cast(int)input.length, cast(ubyte*)input.ptr, output.ptr);
		gpio_disable_all(SlaveIndex.AllMask);
		ubyte s = checksum(output.ptr, cast(ubyte)(output.length - 1));
		if(s == output[$ - 1]) {
			return output[0 .. $-1];
		}
		else {
			return [];
		}
	}

	private:
		int spi_fd;
		ubyte[BUFFER_SIZE] buffer;
}


