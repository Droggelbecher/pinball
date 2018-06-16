
project(font_reader)

string(REGEX REPLACE "([^;]+)" "../\\1" base_sources "${common_sources}")

set(font_reader_sources
	${base_sources}
	pcf_font.cc
	font_reader.cc
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

