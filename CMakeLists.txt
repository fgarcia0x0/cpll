cmake_minimum_required(VERSION 3.12.0)
project(pll VERSION 0.0.2)

include(CTest)

message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE Generator: ${CMAKE_GENERATOR}")

if (NOT MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wpedantic -Wextra -Wno-unused-function -Wno-unused-parameter")
endif()

find_package(GTest CONFIG REQUIRED)

file(GLOB_RECURSE PLL_SOURCES RELATIVE ${PROJECT_SOURCE_DIR} "src/*.cpp")
file(GLOB_RECURSE PLC_SOURCES RELATIVE ${PROJECT_SOURCE_DIR} "test/*.cpp")

add_library(pll SHARED ${PLL_SOURCES})
target_include_directories(pll PUBLIC ${PROJECT_SOURCE_DIR}/include)

enable_testing()

IF (BUILD_TESTING)
	add_executable(pll_test_coverage ${PLC_SOURCES})
	target_link_libraries(pll_test_coverage PRIVATE pll GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)
	include(GoogleTest)
	gtest_discover_tests(pll_test_coverage)
ENDIF(BUILD_TESTING)

set_target_properties(pll pll_test_coverage PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
)