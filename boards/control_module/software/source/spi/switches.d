
import bit_sensor;

enum SwitchesIndex {
	SW_01 = 1,
	SLINGSHOT1 = 2,
	FLIPPER_RIGHT = 3,
	FLIPPER_LEFT = 4,
	SLINGSHOT0 = 5,
	SW_06 = 6,
	SW_07 = 7,

	DTB0_0 = 8,
	DTB0_1 = 9,
	DTB0_2 = 10,
	DTB0_3 = 12,
	DTB0_4 = 11,
	BUMPER0 = 13,
	BUMPER1 = 14,
	BUMPER2 = 15,

	BALL_OUT = 22,
	HOLE0 = 23,

	MAX = 64
};

alias Switches(Spi) = BitSensor!(Spi, SwitchesIndex, Spi.SlaveIndex.Switches);

