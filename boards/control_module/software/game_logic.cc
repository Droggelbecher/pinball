
#include "game_logic.h"
#include <iostream>
#include <iomanip>

GameLogic::GameLogic() {

}

void GameLogic::next_frame() {
	using Sw = Switches::Index;
	using Sol = Solenoids::Index;
	using std::cout;
	using std::endl;


	switches_delta.next_frame();
	solenoids.next_frame();
	//solenoids_delta.next_frame();
	//marquee.next_frame();

	uint8_t fl_left = switches.get(Sw::FLIPPER_LEFT);
	uint8_t fl_right = switches.get(Sw::FLIPPER_RIGHT);

	solenoids.set(Sol::FLIPPER_LEFT, fl_left);
	solenoids.set(Sol::FLIPPER_RIGHT, fl_right);

	if(switches_delta.rising(Sw::FLIPPER_LEFT)) {
		cout << "left" << endl;
	}

	if(switches_delta.rising(Sw::FLIPPER_RIGHT)) {
		cout << "right" << endl;
	}

	//display.clear();

	//std::string text = "Hello, World!";

	//marquee.resize(font_normal.get_size(text));
	//font_normal.render(text, marquee);

	solenoids.set(Sol::DTB0,
		switches.get(Sw::DTB0_0) &&
		switches.get(Sw::DTB0_1) &&
		switches.get(Sw::DTB0_2) &&
		switches.get(Sw::DTB0_3) &&
		switches.get(Sw::DTB0_4));
	
}


