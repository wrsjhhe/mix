cmake_minimum_required(VERSION 3.13) # CMake version check
project(mix)                         # Create project "mix"
set(CMAKE_CXX_STANDARD 20)           # Enable C++20 standard

set(THIRDPARTY_DIR "thirdparty/")

add_executable(mix "main.cpp")

if(EMSCRIPTEN)
  add_definitions(-D__EMSCRIPTEN__)
  set_target_properties(mix  PROPERTIES SUFFIX ".html")
  target_link_options(mix  PRIVATE "-sUSE_WEBGPU=1" "-sUSE_GLFW=3")
else()
  set(DAWN_FETCH_DEPENDENCIES ON)
  add_subdirectory(${THIRDPARTY_DIR}dawn EXCLUDE_FROM_ALL)
  target_link_libraries(mix  PRIVATE webgpu_cpp webgpu_dawn webgpu_glfw)
endif()
