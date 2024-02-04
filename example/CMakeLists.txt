set(publicHeaderDir ${PROJECT_SOURCE_DIR}/include/)

include_directories(${publicHeaderDir})

add_executable(app "main.cpp")
target_link_libraries(app PRIVATE mix)
if(EMSCRIPTEN)
	add_definitions(-D_XM_NO_INTRINSICS_)
	add_definitions(-D__EMSCRIPTEN__)
	set_target_properties(app  PROPERTIES SUFFIX ".html")
	target_link_options(app  PRIVATE "-sUSE_WEBGPU=1" "-sUSE_GLFW=3")
endif()

