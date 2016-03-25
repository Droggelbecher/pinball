
cmake_minimum_required(VERSION 2.6)

project(Pinball)

set(my_SOURCES
	src/audio.cc
	src/canvas/display_canvas.cc
	src/canvas/canvas_buffer.cc
	src/canvas/scrolling_canvas.cc
	src/framer.cc
	src/game_logic.cc
	src/main.cc
	src/switches.cc
	src/utils.cc
	../../../common_software/src/checksum.c
	)

#
# Includes
#

include_directories(src/)
include_directories(../../../common_software/include)

#
# SPI
#

option(MOCK_SPI "Substitute SPI interface with debugging implementation" OFF)
if(MOCK_SPI)
	set(my_SOURCES ${my_SOURCES} src/mock_spi/spi.cc)
	include_directories(src/mock_spi)

else()
	set(my_SOURCES ${my_SOURCES} src/linux_spi/spi.cc)
	include_directories(src/linux_spi)

endif(MOCK_SPI)

#
# Display emulation, find curses if necessary
#

if(MOCK_SPI)
	set(my_SOURCES ${my_SOURCES} src/curses_display/display.cc)
	include_directories(src/curses_display)

	set(CURSES_NEED_NCURSES TRUE)
	find_package(curses REQUIRED)
	include_directories(${CURSES_INCLUDE_DIR})

else()
	set(my_SOURCES ${my_SOURCES} src/spi_display/display.cc)
	include_directories(src/spi_display)

endif()

#
# Executable
#

add_executable(Pinball ${my_SOURCES})

#
# Force C++11
#

target_compile_features(Pinball PRIVATE cxx_uniform_initialization)

#
# Audio libs
#

find_package(OpenAL REQUIRED)

#message(STATUS "OpenAL Lib: " ${OPENAL_LIBRARY})

include_directories(${OPENAL_INCLUDE_DIRS})
target_link_libraries(Pinball ${OPENAL_LIBRARY})

include_directories(/usr/local/opt/alure/include/OpenAL)

find_library(ALURE_LIBRARIES alure PATHS)
target_link_libraries(Pinball ${ALURE_LIBRARIES})

#
# Curses lib (if necessary)
#
if(MOCK_SPI)
	target_link_libraries(Pinball ${CURSES_LIBRARY})
endif()


