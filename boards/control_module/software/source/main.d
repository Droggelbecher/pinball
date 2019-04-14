
import std.stdio: writeln;
import std.datetime;
import std.format;
import core.thread;
import std.experimental.logger;

import story : Story;
import scheduler : Scheduler;

import curses_interface;

import mock_spi: Spi;
//import bcm2708_spi: Spi;

import switches;
import solenoids;
import led_actuator;
import display;
import task;
import coordinate;

void test_spi() {
	import bcm2708_spi: Spi;
	auto s = new Spi();
}

void test_led() {
	alias LED = LEDStripe!Spi;

	auto s = new Spi();
	auto led = new LED(s);

	//led.full([0xff, 0x00, 0x00]);
	//led.frame_start(10.msecs);
	//led.full([0x00, 0xff, 0x00]);
	//led.frame_start(10.msecs);
	enum {
		WHITE = [0xff, 0xff, 0xff],
		YELLOW = [0xf0, 0x60, 0x00],
		BLACK = [0x00, 0x00, 0x30],
	}

	//led.full(YELLOW); //.dt(10);

	led.rotmod(YELLOW, 5, 100);
	while(true) {
		//led.full(WHITE);
		//led.frame_start(40.msecs);
		//Thread.sleep(40.msecs);

		//led.full(BLACK);
		//led.frame_start(40.msecs);
		//Thread.sleep(40.msecs);

		//led.rotmod(YELLOW, 5, 100);
		led.frame_start(40.msecs);
		Thread.sleep(40.msecs);
	}
}

void init_logging(Logger iface_logger) {

	auto time = Clock.currTime;
	auto filename = format!"log-%04d-%02d-%02d--%02d-%02d.txt"(
		time.year, time.month, time.day,
		time.hour, time.minute
	);
	auto file_logger = new FileLogger(filename, LogLevel.all);

	auto global_logger = new MultiLogger(LogLevel.all);
	global_logger.insertLogger("iface_logger", iface_logger);
	global_logger.insertLogger("file_logger", file_logger);
	sharedLog = global_logger;
}

void run_game() {
	alias Sol = Solenoids!Spi;
	alias Sw = Switches!Spi;
	alias LED = LEDStripe!Spi;
	alias Disp = Display!Spi;
	alias Iface = CursesInterface!(Sol, Sw, LED, Disp);

	auto iface_logger = new Iface.Logger(LogLevel.info);
	init_logging(iface_logger);

	auto scheduler = new Scheduler();

	auto spi = new Spi();
	auto solenoids = new Sol(spi);
	auto switches = new Sw(spi);
	auto leds = new LED(spi);
	auto disp = new Disp(spi, Coord(16, 8));

	auto iface = new Iface(iface_logger, solenoids, switches, leds, disp);

	auto story = new Story!Iface(iface);

	scheduler.add([solenoids, switches, leds], -100);
	scheduler.add(story, 0);
	scheduler.add(iface, 100);
	scheduler.run();
}

void main() {
	//test_audio();
	//test_spi();
	//test_mad();
	//test_led();
	
	run_game();
}

