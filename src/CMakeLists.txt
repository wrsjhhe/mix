set(publicHeaderDir ${PROJECT_SOURCE_DIR}/include/)

include_directories(${publicHeaderDir})

set(publicHeaders 
	
	"math/Vector3.h"
	"math/Matrix4.h"

)
set(publicHeadersFull)
foreach (header IN LISTS publicHeaders)
    list(APPEND publicHeadersFull "${publicHeaderDir}${header}")
endforeach ()

set(sources 
	"math/Vector3.cpp"
	"math/Matrix4.cpp"
)

add_library(mix ${sources} ${publicHeadersFull})

if(EMSCRIPTEN)
	add_definitions(-D_XM_NO_INTRINSICS_)
	add_definitions(-D__EMSCRIPTEN__)
	set_target_properties(mix  PROPERTIES SUFFIX ".js")
	target_link_options(mix  PRIVATE "-sUSE_WEBGPU=1" "-sUSE_GLFW=3")
else()
	target_compile_options(mix PRIVATE "/arch:AVX2")
	target_link_libraries(mix  PUBLIC webgpu_cpp webgpu_dawn webgpu_glfw)
endif()