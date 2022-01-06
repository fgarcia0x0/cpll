cmake_minimum_required(VERSION 3.12.0)
project(pll VERSION 0.0.2)

include(CTest)

message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE Generator: ${CMAKE_GENERATOR}")
message(STATUS "Build Test: ${PLL_ENABLE_BUILD_TESTING}")

if (NOT MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wpedantic -Wextra -Wno-unused-function -Wno-unused-parameter")
endif()

# set(PLL_ENABLE_BUILD_TESTING True)

if (PLL_ENABLE_BUILD_TESTING)
	find_package(GTest CONFIG REQUIRED)
endif()

file(GLOB_RECURSE PLL_SOURCES RELATIVE ${PROJECT_SOURCE_DIR} "src/*.cpp")
file(GLOB_RECURSE PLC_SOURCES RELATIVE ${PROJECT_SOURCE_DIR} "test/*.cpp")
file(GLOB_RECURSE TEST_EX_SOURCES RELATIVE ${PROJECT_SOURCE_DIR} "test_ex/*.cpp")

add_library(pll SHARED ${PLL_SOURCES})
target_include_directories(pll PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_executable(test_cnf ${TEST_EX_SOURCES})
target_link_libraries(test_cnf PRIVATE pll)
set_target_properties(test_cnf PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
)

if (PLL_ENABLE_BUILD_TESTING)
	enable_testing()
	add_executable(pll_test_coverage ${PLC_SOURCES})
	target_link_libraries(pll_test_coverage PRIVATE pll GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)
	include(GoogleTest)
	gtest_discover_tests(pll_test_coverage)

	set_target_properties(pll_test_coverage PROPERTIES
		CXX_STANDARD 17
		CXX_STANDARD_REQUIRED ON
		CXX_EXTENSIONS OFF
	)
endif()

set_target_properties(pll PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
)