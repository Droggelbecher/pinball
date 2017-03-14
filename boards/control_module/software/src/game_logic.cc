
#include "game_logic.h"
#include "canvas/paint.h"
#include <iostream>

GAME_LOGIC_TEMPL()::GameLogic(Interface& interface) : interface(interface) {
	marquee.resize({ 16, 100 });

	sound_r2d2_again = audio.sound_load("resources/sounds/r2d2_again.mp3");
	sound_death_star_explode = audio.sound_load("resources/sounds/death_star_explode.mp3");
}

GAME_LOGIC_TEMPL(void)::next_frame() {
	using Sw = typename Interface::SwitchesIndex;
	using Sol = typename Interface::SolenoidsIndex;
	using std::cout;
	using std::endl;

	interface.next_frame();
	framer.next_frame();
	switches_delta.next_frame();
	//marquee.next_frame();
	audio.update();

	//
	// Playfield logic
	//

	// Flipper switches control flippers

	interface.set_solenoid(Sol::FLIPPER_LEFT, !interface.get_switch(Sw::FLIPPER_LEFT));
	interface.set_solenoid(Sol::FLIPPER_RIGHT, !interface.get_switch(Sw::FLIPPER_RIGHT));

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

	interface.set_solenoid(Sol::SLINGSHOT0, !interface.get_switch(Sw::SLINGSHOT0));
	interface.set_solenoid(Sol::SLINGSHOT1, !interface.get_switch(Sw::SLINGSHOT1));

	// Bumper
	
	interface.set_solenoid(Sol::BUMPER0, !interface.get_switch(Sw::BUMPER0));
	interface.set_solenoid(Sol::BUMPER1, !interface.get_switch(Sw::BUMPER1));
	interface.set_solenoid(Sol::BUMPER2, !interface.get_switch(Sw::BUMPER2));

	// If all drop targets are down, re-set them.

	interface.set_solenoid(Sol::DTB0,
		!interface.get_switch(Sw::DTB0_0) &&
		!interface.get_switch(Sw::DTB0_1) &&
		!interface.get_switch(Sw::DTB0_2) &&
		!interface.get_switch(Sw::DTB0_3) &&
		!interface.get_switch(Sw::DTB0_4));

	// Ball return
	interface.set_solenoid(Sol::BALL_RETURN, ball_return.get());

	//
	// Display
	//

	paint_clear(interface);

	marquee.next_frame();
	std::string text = "Hallo, Welt!";
	font_normal.paint_string(marquee, text.c_str(), Coordinate<>(0, 0), 1);

	framer.wait_frame_end();
	
}


