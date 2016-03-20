
#include "game_logic.h"
#include <iostream>

GameLogic::GameLogic() {
}

void GameLogic::next_frame() {
	using Sw = Switches::Index;
	using Sol = Solenoids::Index;
	using std::cout;
	using std::endl;

	//
	// Trigger various sub-components
	//

	switches_delta.next_frame();
	//marquee.next_frame();
	audio.update();

	//
	// Playfield logic
	//

	// Flipper switches control flippers

	solenoids.set(Sol::FLIPPER_LEFT, !switches.get(Sw::FLIPPER_LEFT));
	solenoids.set(Sol::FLIPPER_RIGHT, !switches.get(Sw::FLIPPER_RIGHT));

	if(switches_delta.changed()) {
		cout << switches.get_bits() << endl;

		//for(int i = 0; i < 16; i++) {
			//if(switches.get_bits()[i] != switches_delta.get_previous_bits()[i]) {
				//cout << i << endl;
			//}
		//}
	}

	if(switches_delta.falling(Sw::FLIPPER_LEFT)) {
		cout << "left" << endl;
	}

	if(switches_delta.falling(Sw::FLIPPER_RIGHT)) {
		cout << "right" << endl;
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

	solenoids.next_frame();

	//
	// Display
	//

	//display.clear();

	//std::string text = "Hello, World!";

	//marquee.resize(font_normal.get_size(text));
	//font_normal.render(text, marquee);

	
}

