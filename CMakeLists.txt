cmake_minimum_required(VERSION 3.12.0)
project(pll VERSION 0.0.2)

include(CTest)

message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE Generator: ${CMAKE_GENERATOR}")

if (NOT MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wpedantic -Wextra -Wno-unused-function -Wno-unused-parameter")
endif()

file(GLOB_RECURSE PLL_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")
file(GLOB_RECURSE PLC_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "test/*.cpp")

add_library(pll SHARED ${PLL_SOURCES})
add_executable(plc ${PLC_SOURCES})

target_include_directories(pll PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(plc pll)

## find_package(GTest CONFIG REQUIRED)
## target_link_libraries(main PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)

set_target_properties(pll plc PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
)
