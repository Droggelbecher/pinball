
cmake_minimum_required(VERSION 2.8)

#set(CMAKE_INCLUDE_PATH "/usr/include")
set(CMAKE_BUILD_TYPE Debug CACHE STRING "CMake Build Type (Debug/Release)")

project(Pinball)


set(my_SOURCES
	src/canvas/canvas_buffer.cc
	src/framer.cc
	src/main.cc
	src/pcf_font.cc
	../../../common_software/src/checksum.c
	../../../common_software/src/utils.c
	)

#
# For profiling (gprof)
#
option(ENABLE_PROFILING "Enable generation of profiling information for gprof (-pg)" OFF)
if(ENABLE_PROFILING)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
	SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
	SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")
endif(ENABLE_PROFILING)

# Disable assertions
option(ENABLE_ASSERTIONS "Enable assertions" OFF)
if(ENABLE_ASSERTIONS)
else()
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNDEBUG")
endif(ENABLE_ASSERTIONS)


#
# Includes
#

include_directories(src/)
include_directories(../../../common_software/include)

#
# SPI
#

option(WITH_SPI "Use actual SPI implemntation of OS (otherwise mock)" OFF)
if(WITH_SPI)
	set(my_SOURCES ${my_SOURCES} src/spi/linux_spi.cc)
	set(my_SOURCES ${my_SOURCES} src/spi/switches.cc)
	include_directories(src/linux_spi)

else()
	add_definitions(-DMOCK_SPI)

endif(WITH_SPI)

#
# Audio libs
#

option(WITH_OPENAL_AUDIO "Include OpenAL based music/sound" ON)
if(WITH_OPENAL_AUDIO)
	set(my_SOURCES ${my_SOURCES} src/audio/openal_audio.cc)

	add_definitions(-DWITH_OPENAL_AUDIO)
endif(WITH_OPENAL_AUDIO)

#
# Curses lib (if necessary)
#
option(WITH_CURSES_UI "Add a curses UI for debugging" OFF)

#
# Executable
#

add_executable(Pinball ${my_SOURCES})

#
# Force C++11
#

# Not very portable but works for old cmake versions
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Requires cmake >= ??? (does not seem to work with 2.8)
#target_compile_features(Pinball PRIVATE cxx_uniform_initialization)

# Requires cmake >= 3.1.0
set_property(TARGET Pinball PROPERTY CXX_STANDARD 11)
set_property(TARGET Pinball PROPERTY CXX_STANDARD_REQUIRED ON)


#
# Linking
#

if(WITH_CURSES_UI)
	add_definitions(-DCURSES_UI)
	set(CURSES_NEED_NCURSES TRUE)
	find_package(Curses REQUIRED)
	include_directories(${CURSES_INCLUDE_DIR})
	target_link_libraries(Pinball ${CURSES_LIBRARY})
endif(WITH_CURSES_UI)

if(WITH_OPENAL_AUDIO)
	find_package(OpenAL REQUIRED)

	include_directories(${OPENAL_INCLUDE_DIRS})
	target_link_libraries(Pinball ${OPENAL_LIBRARY})

	# MacOS
	include_directories(/usr/local/opt/alure/include/OpenAL)
	# ArchLinux
	include_directories(/usr/include/AL)

	find_library(ALURE_LIBRARIES alure PATHS)
	target_link_libraries(Pinball ${ALURE_LIBRARIES})
endif(WITH_OPENAL_AUDIO)

#
# Sys libs
#

#target_link_libraries(Pinball -lrt)
#target_link_libraries(Pinball -lm)



