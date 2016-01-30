
#include "gamelogic.h"

Gamelogic::Gamelogic() {

}

void Gamelogic::next_frame() {
	using Sw = Switches::SwitchIndices;
	using Sol = Solenoids::SolenoidIndices;
	using std::cout;
	using std::endl;

	switches_delta.next_frame();
	solenoids_delta.next_frame();
	marquee.next_frame();


	uint8_t fl_left = switches[Sw::FLIPPER_LEFT];
	uint8_t fl_right = switches[Sw::FLIPPER_RIGHT];

	solenoids[Sol::LEFT] = fl_left;
	solenoids[Sol::RIGHT] = fl_right;

	if(switches_delta.rising(Sw::FLIPPER_LEFT)) {
		cout << "left" << endl;
	}

	if(switches_delta.rising(Sw::FLIPPER_RIGHT)) {
		cout << "right" << endl;
	}

	display.clear();

	std::string text = "Hello, World!";

	marquee.resize(font_normal.get_size(text));
	font_normal.render(text, marquee);

	solenoids[Sol::DTB0] = switches.range_closed(Sw:DTB0, 5);
}


