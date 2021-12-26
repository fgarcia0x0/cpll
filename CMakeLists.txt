cmake_minimum_required(VERSION 3.12.0)
project(pll VERSION 0.0.1)

include(CTest)

message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
if (NOT MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wpedantic -Wextra -Wno-unused-function -Wno-unused-parameter")
endif()

file(GLOB_RECURSE CPLL_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")
add_executable(pll ${CPLL_SOURCES})

enable_testing()

# add_test(NAME plc COMMAND plc)

set_target_properties(pll PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
)
