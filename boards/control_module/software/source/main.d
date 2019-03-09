
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
import task;
import audio;
//import audio = mock_audio;

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
	import logger: WritelnLogger;

	auto logger = new WritelnLogger;
	auto audio_interface = AudioInterface(logger);
	auto scheduler = new Scheduler(logger);

	auto sound = new Sound(logger, "./resources/music/original/01_IV_main_theme.mp3"); // joint stereo
	//auto sound = new Sound(logger, "./resources/sounds/utini.mp3");
	//auto sound = new Sound(logger, "./resources/sounds/missile_alert.mp3"); // mono
	//auto sound = new Sound(logger, "./resources/sounds/alert.mp3"); // joint stereo

	auto playlist = new Playlist(
		logger,
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

void run_game() {
	alias Sol = Solenoids!Spi;
	alias Sw = Switches!Spi;
	alias Iface = CursesInterface!(Sol, Sw);

	auto spi = new Spi();
	auto iface = new Iface(new Sol(spi), new Sw(spi));

	//audio.init();

	auto scheduler = new Scheduler(iface.logger);
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

