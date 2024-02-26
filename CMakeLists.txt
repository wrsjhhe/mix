cmake_minimum_required(VERSION 3.13) # CMake version check
project(mix)                         # Create project "mix"
set(CMAKE_CXX_STANDARD 17)           # Enable C++17 standard

set(THIRDPARTY_DIR ${PROJECT_SOURCE_DIR}/thirdparty/)

set(DAWN_FETCH_DEPENDENCIES ON)

if(NOT EMSCRIPTEN)
	include(E:/git_download/vcpkg/scripts/buildsystems/vcpkg.cmake)
	#include(E:/vcpkg/scripts/buildsystems/vcpkg.cmake)
	include_directories(${CMAKE_CURRENT_BINARY_DIR}/vcpkg_installed/x64-windows/include/)
	link_directories(${CMAKE_CURRENT_BINARY_DIR}/vcpkg_installed/x64-windows/lib/)
endif()

# set(WEBGPU_BACKEND DAWN)
# add_definitions(-DWEBGPU_CPP_IMPLEMENTATION)

add_subdirectory(webgpu)
add_subdirectory(glfw3webgpu)
add_subdirectory(src)
add_subdirectory(example)