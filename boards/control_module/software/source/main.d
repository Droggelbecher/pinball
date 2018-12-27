
import std.stdio: writeln;
import std.datetime;

import game_logic : GameLogic;
import scheduler : Scheduler;

import curses_interface;
import mock_spi;
import switches;
import solenoids;
import task;
import core.thread;

void main() {
	auto iface = new CursesInterface();
	auto scheduler = new Scheduler();

	auto spi = new Spi();
	auto solenoids = new Solenoids!(Spi)(spi);
	auto switches = new Switches!(Spi)(spi);

	auto logic = new GameLogic!(
			Solenoids!(Spi),
			Switches!(Spi),
			CursesInterface,
	)(
		solenoids,
		switches,
		iface
	);

	scheduler.tasks ~= logic;
	scheduler.tasks ~= iface;
	//scheduler.tasks ~= new Task(10.seconds);
	scheduler.run();
}

