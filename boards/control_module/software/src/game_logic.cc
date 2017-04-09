
#include "game_logic.h"
#include "canvas/paint.h"
#include <iostream>

GAME_LOGIC_TEMPL()::GameLogic(Interface& interface) : interface(interface) {
	marquee.resize({ 16, 100 });

	sound_r2d2_again = audio.sound_load("resources/sounds/r2d2_again.mp3");
	sound_death_star_explode = audio.sound_load("resources/sounds/death_star_explode.mp3");
}

GAME_LOGIC_TEMPL(void)::next_frame() {
	using Sw = typename Interface::Switches::Index;
	using Sol = typename Interface::Solenoids::Index;
	using std::cout;
	using std::endl;

	static typename Interface::Switches& switches = interface.switches();
	static typename Interface::Solenoids& solenoids = interface.solenoids();
	static typename Interface::Lamps& lamps = interface.lamps();

	interface.next_frame();
	framer.next_frame();
	switches_delta.next_frame();
	audio.update();

	ball_return.next_frame();

	//
	// Playfield logic
	//

	// Flipper switches control flippers

	solenoids.set(Sol::FLIPPER_LEFT, !switches.get(Sw::FLIPPER_LEFT));
	solenoids.set(Sol::FLIPPER_RIGHT, !switches.get(Sw::FLIPPER_RIGHT));

	/*
	if(switches_delta.changed()) {
		cout << interface.get_switches_bits() << endl;

		for(int i = 0; i < switches.get_bits().size(); i++) {
			if(switches.get_bits()[i] != switches_delta.get_previous_state()[i]) {
				cout << i
					<< (switches.get_bits()[i] ? " ->high" : " ->low")
					<< endl;
			}
		}

		cout << "bumpers: "
			<< switches.get(Sw::BUMPER0)
			<< switches.get(Sw::BUMPER1)
			<< switches.get(Sw::BUMPER2)
			<< endl;

		cout << "drop target bank 0: "
			<< switches.get(Sw::DTB0_0)
			<< switches.get(Sw::DTB0_1)
			<< switches.get(Sw::DTB0_2)
			<< switches.get(Sw::DTB0_3)
			<< switches.get(Sw::DTB0_4)
			<< endl;
	}

	if(switches_delta.falling(Sw::FLIPPER_LEFT)) {
		cout << "left" << endl;
	}

	if(switches_delta.falling(Sw::FLIPPER_RIGHT)) {
		cout << "right" << endl;
	}
	*/

	//
	if(switches_delta.falling(Sw::HOLE0)) {
		audio.sound_play(sound_death_star_explode);
	}


	// Slingshot switches control slingshots

	solenoids.set(Sol::SLINGSHOT0, !switches.get(Sw::SLINGSHOT0));
	solenoids.set(Sol::SLINGSHOT1, !switches.get(Sw::SLINGSHOT1));

	// Bumper
	
	solenoids.set(Sol::BUMPER0, !switches.get(Sw::BUMPER0));
	solenoids.set(Sol::BUMPER1, !switches.get(Sw::BUMPER1));
	solenoids.set(Sol::BUMPER2, !switches.get(Sw::BUMPER2));

	// If all drop targets are down, re-set them.

	solenoids.set(Sol::DTB0,
		!switches.get(Sw::DTB0_0) &&
		!switches.get(Sw::DTB0_1) &&
		!switches.get(Sw::DTB0_2) &&
		!switches.get(Sw::DTB0_3) &&
		!switches.get(Sw::DTB0_4));

	// Ball return
	solenoids.set(Sol::BALL_RETURN, !ball_return.get());

	//
	// Display
	//

	paint_clear(interface.canvas());

	marquee.next_frame();
	std::string text = "Hallo, Welt!";
	font_normal.paint_string(marquee, text.c_str(), Coordinate<>(0, 0), 1);

	framer.wait_frame_end();
	
}


