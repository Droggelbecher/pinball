
import std.stdio: writeln;
import std.datetime;
import core.thread;

import game_logic : GameLogic;
import scheduler : Scheduler;

import curses_interface;
import mock_spi: Spi;
//import bcm2708_spi: Spi;
import switches;
import solenoids;
import led_actuator;
import task;
import audio;
//import audio = mock_audio;
import std.experimental.logger;

void test_audio() {
	//audio.init();
	//auto main_theme = new SoundEffect("./resources/music/original/01_IV_main_theme.mp3");
	//main_theme.play();
}

void test_spi() {
	import bcm2708_spi: Spi;
	auto s = new Spi();
}

void test_mad() {
	import audio_mad_al;

	auto audio_interface = new AudioInterface();
	auto scheduler = new Scheduler();

	auto sound = new Sound("./resources/music/original/01_IV_main_theme.mp3"); // joint stereo
	//auto sound = new Sound("./resources/sounds/utini.mp3");
	//auto sound = new Sound("./resources/sounds/missile_alert.mp3"); // mono
	//auto sound = new Sound("./resources/sounds/alert.mp3"); // joint stereo

	auto playlist = new Playlist(
		"./resources/sounds/blip1.mp3",
		"./resources/sounds/alert.mp3",
		"./resources/sounds/chime.mp3",
		"./resources/sounds/utini.mp3",
	);


	class MyTask: Task {
		override void run() {
			writeln("Playlist!");
			playlist.play();
			yield(1000.msecs);
			writeln("Uitini!");
			sound.play();
		}
	}


	scheduler.add(playlist);
	//scheduler.add(music);
	scheduler.add(sound);
	scheduler.add(new MyTask);
	scheduler.run();
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
	alias Iface = CursesInterface!(Sol, Sw, LED);

	auto iface_logger = new Iface.Logger(LogLevel.info);
	init_logging(iface_logger);

	auto spi = new Spi();
	auto iface = new Iface(iface_logger, new Sol(spi), new Sw(spi), new LED(spi));


	//audio.init();

	auto scheduler = new Scheduler();
	auto logic = new GameLogic!Iface(iface);

	scheduler.add(logic);
	scheduler.add(iface);
	scheduler.run();
}

void main() {
	//test_audio();
	//test_spi();
	run_game();
	//test_mad();
}

