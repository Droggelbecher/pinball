
enable_testing()

find_package(Boost COMPONENTS unit_test_framework REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

include_directories(../src)
include_directories(../tools)
include_directories(../../../../common_software/include)

file(GLOB TEST_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cc)

set(base_sources
	../src/canvas/buffer.cc
	../tools/pcf_font.cc
	../../../../common_software/src/checksum.c
	../../../../common_software/src/utils.c
	)

foreach(filename ${TEST_SRCS})
	get_filename_component(test_name ${filename} NAME_WE)

	add_executable(${test_name} ${filename} ${base_sources})
	target_link_libraries(${test_name} ${Boost_LIBRARIES})

	set_target_properties(${test_name} PROPERTIES 
		RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_CURRENT_SOURCE_DIR}/bin)

	add_test(NAME ${test_name} 
			 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
			 COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/bin/${test_name} )

	set_property(TARGET ${test_name} PROPERTY CXX_STANDARD 14)
	set_property(TARGET ${test_name} PROPERTY CXX_STANDARD_REQUIRED ON)
endforeach(filename)

