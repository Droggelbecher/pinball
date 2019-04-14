
#ifndef MAIN_H
#define MAIN_H

#include "config.h"

#if MOCK_SPI
	#include "game_interface/dummy_interface.h"

	namespace pinball {
		using SpiInterface_t = DummyInterface;
	}
#else
	#include "spi/spi.h"
	#include "spi/spi_interface.h"

	namespace pinball {
		using SpiInterface_t = SpiInterface;
	}
#endif

#if CURSES_UI
	//#include "game_interface/curses_interface.h"
	
	namespace pinball {
		template<typename T>
		class CursesInterface;

		using Interface = CursesInterface<SpiInterface_t>;
	}
#else
	namespace pinball {
		using Interface = SpiInterface_t;
	}
#endif

namespace pinball {
	//extern Interface interface;
	Interface& interface();
}


#endif // MAIN_H

