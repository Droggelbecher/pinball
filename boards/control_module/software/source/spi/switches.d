
import bit_sensor;

enum SwitchesIndex {
	SW_00 = 0,
	SW_01 = 1,
	SLINGSHOT1 = 2, // Right
	FLIPPER_RIGHT = 3,
	FLIPPER_LEFT = 4,
	SLINGSHOT0 = 5, // Left
	SW_07 = 7,
	SPINNER = 6,

	DTB0_0 = 8,
	DTB0_1 = 9,
	DTB0_2 = 10,
	DTB0_3 = 11,
	DTB0_4 = 12,
	BUMPER0 = 13,
	BUMPER1 = 14,
	BUMPER2 = 15,

	BALL_OUT = 22,
	HOLE0 = 23,

	MAX = 64
};

alias Switches(Spi) = BitSensor!(Spi, SwitchesIndex, Spi.SlaveIndex.Switches);

