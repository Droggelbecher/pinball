
project(font_reader)

set(font_reader_sources
	pcf_font.cc
	font_reader.cc
	../src/canvas/buffer.cc
	../../../../common_software/src/checksum.c
	../../../../common_software/src/utils.c
	)

#
# Includes
#

include_directories(./)
include_directories(../src/)
include_directories(../../../../common_software/include)

add_executable(font_reader ${font_reader_sources})
set_property(TARGET font_reader PROPERTY CXX_STANDARD 14)
set_property(TARGET font_reader PROPERTY CXX_STANDARD_REQUIRED ON)

