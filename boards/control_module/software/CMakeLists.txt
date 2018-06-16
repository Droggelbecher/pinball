
cmake_minimum_required(VERSION 3.1)

# Source files common to the implementation and tests
set(common_sources
	src/canvas/buffer.cc
	../../../common_software/src/checksum.c
	../../../common_software/src/utils.c
)

add_subdirectory(src)
add_subdirectory(tools)
add_subdirectory(test)

