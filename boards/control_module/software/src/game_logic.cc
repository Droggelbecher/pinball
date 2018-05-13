
#include "game_logic.h"
#include "canvas/canvas.h"

#include <iostream>
#include <iomanip>
#include <sstream>

namespace pinball {

GAME_LOGIC_TEMPL()::GameLogic(Interface& interface) : interface(interface) {
	//marquee.resize({ 16, 100 });
	// TODO


	sound_r2d2_again = audio.sound_load("resources/sounds/r2d2_again.mp3");
	sound_death_star_explode = audio.sound_load("resources/sounds/death_star_explode.mp3");
}

GAME_LOGIC_TEMPL(void)::next_frame(double dt) {
	using Sw = typename Interface::Switches::Index;
	using Sol = typename Interface::Solenoids::Index;
	using std::cout;
	using std::endl;

	static typename Interface::Switches& switches = interface.switches();
	static typename Interface::Solenoids& solenoids = interface.solenoids();
	static typename Interface::Lamps& lamps = interface.lamps();

	static const int max_lamps = 8;
	static int current_lamp = 0;

	interface.next_frame(dt);
	marquee.next_frame(dt);
	switches_delta.next_frame(dt);
	ball_return.next_frame(dt);

	audio.update();

	//
	// Playfield logic
	//

	// Flipper switches control flippers

	solenoids.set(Sol::FLIPPER_LEFT, switches.get(Sw::FLIPPER_LEFT));
	solenoids.set(Sol::FLIPPER_RIGHT, switches.get(Sw::FLIPPER_RIGHT));

	// For testing: FL LEFT cycles through lamps
	if(switches.get(Sw::FLIPPER_LEFT)) {
		++current_lamp;
		current_lamp %= max_lamps;
		lamps.set(false);
		lamps.set(static_cast<typename Interface::Lamps::Index>(current_lamp), true);
	}

	//
	if(switches_delta.rising(Sw::HOLE0)) {
		audio.sound_play(sound_death_star_explode);
	}


	// Slingshot switches control slingshots

	solenoids.set(Sol::SLINGSHOT0, switches.get(Sw::SLINGSHOT0));
	solenoids.set(Sol::SLINGSHOT1, switches.get(Sw::SLINGSHOT1));

	// Bumper
	
	solenoids.set(Sol::BUMPER0, switches.get(Sw::BUMPER0));
	solenoids.set(Sol::BUMPER1, switches.get(Sw::BUMPER1));
	solenoids.set(Sol::BUMPER2, switches.get(Sw::BUMPER2));

	// If all drop targets are down, re-set them.

	solenoids.set(Sol::DTB0,
		switches.get(Sw::DTB0_0) &&
		switches.get(Sw::DTB0_1) &&
		switches.get(Sw::DTB0_2) &&
		switches.get(Sw::DTB0_3) &&
		switches.get(Sw::DTB0_4));

	/*
	std::cerr
		<< "DTB0: "
		<< switches.get(Sw::DTB0_0)
		<< switches.get(Sw::DTB0_1)
		<< switches.get(Sw::DTB0_2)
		<< switches.get(Sw::DTB0_3)
		<< switches.get(Sw::DTB0_4)
		<< std::endl;
	*/

	// Ball return
	solenoids.set(Sol::BALL_RETURN, ball_return.get());

	/*
	std::cerr
		<< "BALL: "
		<< ball_return.get()
		<< std::endl;
	*/

	//
	// Display
	//


	// TODO
	//clear(interface.canvas());

	//paint_pattern(marquee);

	/*
	std::ostringstream ss;
	ss << "FPS: " << std::setprecision(3) << (1.0 / dt); //Hallo, Welt!";
	font_normal.paint_string(marquee, ss.str().c_str(), Coordinate<>(0, 0), 1);
	*/
	
}

} // ns pinball

