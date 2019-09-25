
import bit_actuator;

enum SolenoidsIndex {
	FLIPPER_LEFT  = 0,
	FLIPPER_RIGHT = 1,
	DTB0          = 2,
	SLINGSHOT0    = 3,
	SLINGSHOT1    = 4,
	BUMPER1       = 5,
	BUMPER2       = 6,
	BUMPER0       = 7,
	SOL_08        = 8,
	BALL_RETURN   = 9,
	SOL_10        = 10,
	SOL_11        = 11,
	MAX = 16
}

alias Solenoids(Spi) = BitActuator!(Spi, SolenoidsIndex, Spi.SlaveIndex.Solenoids);

