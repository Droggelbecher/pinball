#ifndef SPI_INTERFACE_H_INCLUDED
#define SPI_INTERFACE_H_INCLUDED

#include "spi_display.h"
#include "spi/switches.h"
#include "spi/solenoids.h"
#include "spi/lamps.h"
#include "canvas/canvas.h"

// TODO
// Make this work, take DummyInterface and CursesInterface as example.

class SpiInterface {
	public:
		using Switches = ::Switches;
		using Solenoids = ::Solenoids;
		using Lamps = ::Lamps;
		using Canvas = ::SpiDisplay;

		SpiInterface(Spi& spi, int modules, Coordinate<> module_size) :
			canvas_(spi, modules, module_size),
			switches_(spi),
			solenoids_(spi),
			lamps_(spi)
		{ }

		void next_frame(double dt) {
			switches().next_frame(dt);
			solenoids().next_frame(dt);
			lamps().next_frame(dt);
			canvas().next_frame(dt);
		}

		Switches& switches() { return switches_; }
		Solenoids& solenoids() { return solenoids_; }
		Lamps& lamps() { return lamps_; }
		Canvas& canvas() { return canvas_; }

	private:
		Canvas canvas_;
		Switches switches_;
		Solenoids solenoids_;
		Lamps lamps_;
};


#endif // SPI_INTERFACE_H_INCLUDED

