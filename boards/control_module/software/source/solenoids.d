
import bit_actuator;

enum SolenoidsIndex {
	FLIPPER_LEFT  = 0,
	FLIPPER_RIGHT = 1,
	DTB0          = 2,
	SLINGSHOT0    = 3,
	SLINGSHOT1    = 4,
	BUMPER0       = 5,
	BUMPER1       = 6,
	BUMPER2       = 7,
	BALL_RETURN   = 8,
	AUX_0         = 9,
	AUX_1         = 10,
	MAX = 16
};

alias Solenoids(Spi) = BitActuator!(Spi, SolenoidsIndex, Spi.SlaveIndex.Solenoids);

