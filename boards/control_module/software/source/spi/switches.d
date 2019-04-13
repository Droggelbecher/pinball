
import bit_sensor;

enum SwitchesIndex {
	FLIPPER_LEFT = 2,
	FLIPPER_RIGHT = 5,
	DTB0_0 = 8,
	DTB0_1 = 9,
	DTB0_2 = 10,
	DTB0_3 = 12,
	DTB0_4 = 11,
	SLINGSHOT0 = 3,
	SLINGSHOT1 = 4,
	BUMPER0 = 13,
	BUMPER1 = 14,
	BUMPER2 = 15,
	BALL_OUT = 22,
	HOLE0 = 23,
	MAX = 64
};

alias Switches(Spi) = BitSensor!(Spi, SwitchesIndex, Spi.SlaveIndex.Switches);

