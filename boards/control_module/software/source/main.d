
import std.stdio: writeln;
import std.datetime;
import core.thread;

import game_logic : GameLogic;
import scheduler : Scheduler;

import curses_interface;
import mock_spi;
import switches;
import solenoids;
import task;
import audio;

void test_audio() {
	audio.init();
	AudioSource main_theme = load_sound("./resources/music/original/01_IV_main_theme.mp3");
	main_theme.play();
}

void run_game() {
	audio.init();

	alias Sol = Solenoids!Spi;
	alias Sw = Switches!Spi;
	alias Iface = CursesInterface!(Sol, Sw);

	auto scheduler = new Scheduler();

	auto spi = new Spi();
	//auto solenoids = new Sol(spi);
	//auto switches = new Sw(spi);
	auto iface = new Iface(new Sol(spi), new Sw(spi));

	auto logic = new GameLogic!Iface(iface);

	scheduler.tasks ~= logic;
	scheduler.tasks ~= iface;
	//scheduler.tasks ~= new Task(10.seconds);
	scheduler.run();
}

void main() {
	//test_audio();
	run_game();
}

