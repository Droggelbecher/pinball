
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
	ubyte[] transfer_and_check(SlaveIndex slave, ubyte[] input)
	in {
		assert(input.length < BUFFER_SIZE - 1);
	}
	do {
		gpio_enable_only(1 << slave, SlaveIndex.AllMask);

		//ubyte[input.length] output;
		ubyte[] output = buffer[0 .. input.length];
		output[] = 0xAA;

		spi_transfer(spi_fd, cast(int)input.length, input.ptr, output.ptr);
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

/+

Magic-Karten
- Alles passt futzelmaessig nicht zusammen, keine klare Linie
- Nicht alle Karten passen in eine Kiste "abgeschlossene Einheit" (schubfach, regal)
- Logisch:
	- Innerhalb der Kartons: Was sind sinnvolle Kategorien? Wie kleinteilig sollte es sein?
	- Unterscheidung Rares, Non-Rares, sinnvoll?
- Unterbringung der Masse
- Kein zuhause für 2-farbige
- Angefangene Decks, was wollte ich da denn bauen?
- Abhängig vom Format / Ziel: Just for fun / zu zweit? Casual? Turnier?
- Einzelne Blätter / Ordner wie bei rares gut: Passt in Ikea etc..
	- Kartons: Höhe exotisch, schlecht stapelbar
- Rollbar wäre cool? Oder vllt reich auch gures Ablagesystem

+/

/+
import std.mmfile;

enum Slave {
	LAMPS, SWITCHES, DISPLAY, SOLENOIDS,
	ALL
}

shared class Spi {
	this() {
		open_gpio();
		gpio_set_output(Slave.LAMPS);
		gpio_set_output(Slave.SWITCHES);
		gpio_set_output(Slave.DISPLAY);
		gpio_set_output(Slave.SOLENOIDS);
		open_spi();
	}

	ubyte[] transfer_and_check(Slave slave, ubyte[] input) {
		enable_only(slave);
		ubyte[] output = new ubyte[](input.length);
		output[] = 77;
		transfer(input, output);
		disable_all();
		ubyte s = 1; //checksum(output[0 .. $-1]); // TODO: implement or link
		if(s == output[$ - 1]) {
			return output[0 .. $-1];
		}
		else {
			return output[0 .. 0];
		}
	}

	private:

	int transfer(ubyte[] input, ubyte[] output) {
		//spi_ioc_transfer msg;
		//memset((void*)&msg, 0x00, sizeof(msg));

		// TODO: Link to externel C code for <linux/spi/spidev.h>
		return 0;
	}
}

private:

import core.stdc.errno;
import core.stdc.stdio;
import core.stdc.stdlib;
import core.sys.posix.fcntl;
import core.sys.posix.sys.mman;
import core.sys.posix.sys.stat;
import core.sys.posix.unistd;
import core.sys.posix.sys.ioctl;

enum BCM2708_PERI_BASE = 0x20000000;
enum GPIO_BASE = (BCM2708_PERI_BASE + 0x200000); /* GPIO controller */
enum GPIO_PAGE_SIZE = (4*1024);
enum GPIO_BLOCK_SIZE = (4*1024);
enum SPI_MODE = 0;
enum SPI_BITS_PER_WORD = 8;
enum SPI_BUS_SPEED = 1000 * 1000;
enum SPI_IOC_MAGIC = 'k';


/+
#define GPIO_GET *(gpio_map+13)
+/

void gpio_set_input(ulong g) {
	*(gpio_map + (g / 10)) &= ~(7 << ((g % 10) * 3));
}

void gpio_set_output(ulong g) {
	*(gpio_map + (g / 10)) &= ~(7 << ((g % 10) * 3));
	*(gpio_map + (g / 10)) |=  (1 << ((g % 10) * 3));
}

void enable_only(Slave slave) {
	*(gpio_map + 7) = cast(ubyte)Slave.ALL;
	*(gpio_map + 10) = cast(ubyte)(1 << cast(int)slave);
	//usleep(100UL); // TODO
}

void disable_all() {
	*(gpio_map + 10) = 0;
	*(gpio_map + 7) = Slave.ALL;
	//usleep(100UL); // TODO
}

shared ubyte* gpio_map = null;

void open_gpio() {
	int gpio_mem_fd = open("/dev/mem", O_RDWR | O_SYNC);
	if(gpio_mem_fd < 0) {
		perror("cannot open /dev/mem");
		exit(1);
	}
	scope(exit) { close(gpio_mem_fd); }

	void *p = mmap(
			null,
			GPIO_BLOCK_SIZE,
			PROT_READ | PROT_WRITE,
			MAP_SHARED,
			gpio_mem_fd,
			GPIO_BASE
	);
	if(p == MAP_FAILED) {
		perror("mmap failed");
	}

	gpio_map = cast(shared(ubyte)*)p;
}

void open_spi() {

	int status = -1;
	char spi_mode = SPI_MODE;
	char spi_bits_per_word = SPI_BITS_PER_WORD;
	uint spi_speed = SPI_BUS_SPEED;

	spi_cs_fd = open("/dev/spidev0.0", O_RDWR);
	if(spi_cs_fd < 0) {
		perror("Couldnt open SPI device");
		exit(1);
	}

	//status = ioctl(spi_cs_fd, SPI_IOC_WR_MODE, &spi_mode);
	status = ioctl(spi_cs_fd, _IOW(,1, __u8), &spi_mode);
	if(status < 0) {
		perror("Could not set SPI write mode");
		exit(1);
	}

	status = ioctl(spi_cs_fd, SPI_IOC_RD_MODE, &spi_mode);
	if(status < 0) {
		perror("Could not set SPI read mode");
		exit(1);
	}

	status = ioctl(spi_cs_fd, SPI_IOC_WR_BITS_PER_WORD, &spi_bits_per_word);
	if(status < 0) {
		perror("Couldnt set bits per word (WR)");
		exit(1);
	}

	status = ioctl(spi_cs_fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed);
	if(status < 0) {
		perror("Couldnt set max write speed");
		exit(1);
	}

	
	status = ioctl(spi_cs_fd, SPI_IOC_RD_MAX_SPEED_HZ, &spi_speed);
	if(status < 0) {
		perror("Couldnt set max read speed");
		exit(1);
	}
}

/+
shared static this() {
	open_gpio();
	open_spi();
}

shared static ~this() {
	close_spi();
	close_gpio();
}
+/

+/

