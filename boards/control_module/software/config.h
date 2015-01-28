
//
// Display
//

#define DISPLAY_TARGET_FPS 16.0f

#define DISPLAY_MODULE_COUNT 1
#define DISPLAY_MODULE_ROWS 16
#define DISPLAY_MODULE_COLUMNS 8
#define DISPLAY_MODULE_COLORS 2


//
// SPI
//

#define SPI_MODE 0
#define SPI_BITS_PER_WORD 8

// 1 MHz seems to be what our display module can handle
#define SPI_BUS_SPEED 1 * 1000 * 1000

//
// SPI Slave-Select GPIO pin numbers on pi
//
// This depends on the soldering on the self-soldered raspberry
// SPI extension board
//

#define SPI_SS_PIN_LAMPS 22
#define SPI_SS_PIN_BUTTONS 23
#define SPI_SS_PIN_DISPLAY 24
#define SPI_SS_PIN_SOLENOIDS 25

#define SPI_SS_SLAVES_MASK (\
		(1 << SPI_SS_PIN_LAMPS) | \
		(1 << SPI_SS_PIN_DISPLAY) | \
		(1 << SPI_SS_PIN_BUTTONS) | \
		(1 << SPI_SS_PIN_SOLENOIDS) \
	)


