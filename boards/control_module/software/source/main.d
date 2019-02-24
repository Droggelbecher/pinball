
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
	Logger logger = new WritelnLogger;
	auto audio_interface = AudioInterface(logger);
	auto sound = Sound("./resources/sounds/blip1.mp3");
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

	scheduler.tasks ~= logic;
	scheduler.tasks ~= iface;
	scheduler.run();
}

void main() {
	//test_audio();
	//test_spi();
	run_game();
	//test_mad();
}

