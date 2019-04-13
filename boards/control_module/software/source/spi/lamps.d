
import bit_actuator;

enum LampsIndex {
	DS_WHITE  ,
	DS_WEAPON ,
	TARGET    ,
	SLINGSHOT0,
	SLINGSHOT1,
	BUMPER0,
	BUMPER1,
	BUMPER2
}

alias Lamps(Spi) = BitActuator!(Spi, LampsIndex, Spi.SlaveIndex.Lamps);

