cmake_minimum_required(VERSION 3.12.0)
project(cpll VERSION 0.0.1)

include(CTest)

message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
if (NOT MSVC)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wpedantic -Wextra -Wno-unused-function -Wno-unused-parameter")
endif()

file(GLOB_RECURSE CPLL_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.c")
add_executable(cpll ${CPLL_SOURCES})

enable_testing()

# add_test(NAME plc COMMAND plc)

set_target_properties(cpll PROPERTIES
	C_STANDARD 11
	C_STANDARD_REQUIRED ON
	C_EXTENSIONS OFF
)
