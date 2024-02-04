cmake_minimum_required(VERSION 3.13) # CMake version check
project(mix)                         # Create project "mix"
set(CMAKE_CXX_STANDARD 17)           # Enable C++17 standard

set(THIRDPARTY_DIR ${PROJECT_SOURCE_DIR}/thirdparty/)

set(DAWN_FETCH_DEPENDENCIES ON)

include_directories(${THIRDPARTY_DIR}DirectXMath)

if(NOT EMSCRIPTEN)
	add_subdirectory(${THIRDPARTY_DIR}dawn EXCLUDE_FROM_ALL)
endif()

add_subdirectory("src")
add_subdirectory("example")