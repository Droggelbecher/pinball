#ifndef SPI_INTERFACE_H_INCLUDED
#define SPI_INTERFACE_H_INCLUDED

#include "spi_display.h"
#include "switches.h"
#include "solenoids.h"
#include "lamps.h"

class SpiInterface {

	public:

		SpiInterface(Spi& spi) : spi(spi) { }

		void next_frame() {
			display.next_frame();
			switches.next_frame();
			solenoids.next_frame();
			lamps.next_frame();
		}

		// Display
		using Color = Display::Color;
		Coordinate<> display_size() const { return display.size(); }
		void set_display(Coordinate<> c, uint8_t v) { return display.set(c, v); }
		uint8_t get_display(Coordinate<> c) { return display.get(c); }
		int buffer_length() const { return display.buffer_length(); }
		uint8_t* buffer() { return display.buffer(); }

		// Switches
		using SwitchesIndex = Switches::Index;
		using SwitchesBitset = Switches::Bitset;
		bool get_switch(SwitchesIndex i) const { return switches.get(i); }
		const Bitset& get_switches_bits() const { return switches.get_bits(); }

		// Solenoids
		using SolenoidsIndex = Solenoids::Index;
		void set_solenoid(SolenoidsIndex i, bool v) { return solenoids.set(i, v); }

		// Lamps
		using LampsIndex = Lamps::Index;
		void set_lamp(LampsIndex i, bool v) { return lamps.set(i, v); }

	private:
		Display display;
		Switches switches { spi };
		Solenoids solenoids { spi };
		Lamps lamps { spi };
};


#endif // SPI_INTERFACE_H_INCLUDED

