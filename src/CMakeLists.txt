set(publicHeaderDir ${PROJECT_SOURCE_DIR}/include/)

include_directories(${publicHeaderDir})

set(publicHeaders 

    "canvas/Canvas.h"
    "canvas/WindowSize.h"
	
	"nodes/core/constants.h"
	"nodes/core/NodeFrame.h"

	"core/BufferAttribute.h"
	"core/BufferGeometry.h"
	"core/Clock.h"
	"core/EventDispatcher.h"
	"core/Face3.h"
	"core/Layers.h"
	"core/misc.h"
	"core/InstancedBufferAttribute.h"
	"core/InstancedBufferGeometry.h"
	"core/InterleavedBuffer.h"
	"core/InterleavedBufferAttribute.h"
	"core/Object3D.h"
	"core/Raycaster.h"
	"core/Shader.h"
	"core/Uniform.h"

	"input/KeyListener.h"
    "input/MouseListener.h"
    "input/PeripheralsEventSource.h"

	"renderers/common/nodes/Nodes.h"
	"renderers/common/Info.h"
	"renderers/common/Backend.h"
	"renderers/common/Renderer.h"
	"renderers/common/Attributes.h"
	"renderers/common/Geometries.h"
	"renderers/common/Animation.h"
	"renderers/common/Background.h"
	"renderers/common/Textures.h"
	"renderers/common/Pipelines.h"
	"renderers/common/Bindings.h"
	"renderers/common/RenderObject.h"
	"renderers/common/RenderObjects.h"
	"renderers/common/RenderLists.h"
	"renderers/common/RenderContext.h"
	"renderers/common/RenderContexts.h"

	"renderers/webgpu/utils/WebGPUUtils.h"
	"renderers/webgpu/utils/WebGPUAttributeUtils.h"
	"renderers/webgpu/utils/WebGPUBindingUtils.h"
	"renderers/webgpu/utils/WebGPUPipelineUtils.h"
	"renderers/webgpu/utils/WebGPUTextureUtils.h"

	"renderers/WebGPUBackend.h"
	"renderers/WebGPURenderer.h"
	"renderers/RenderTarget.h"

	"cameras/Camera.h"
	"cameras/PerspectiveCamera.h"
	"cameras/OrthographicCamera.h"

	"materials/Material.h"
	"materials/SpriteMaterial.h"
	"materials/interfaces.h"

	"objects/Sprite.h"

	"lights/lights.h"
	"lights/Light.h"
	"lights/LightProbe.h"
	"lights/AmbientLight.h"
	"lights/DirectionalLight.h"
	"lights/DirectionalLightShadow.h"
	"lights/HemisphereLight.h"
	"lights/LightShadow.h"
	"lights/PointLight.h"
	"lights/PointLightShadow.h"
	"lights/SpotLight.h"
	"lights/SpotLightShadow.h"

	"textures/DataTexture.h"
	"textures/DataTexture3D.h"
	"textures/DepthTexture.h"
	"textures/Image.h"
	"textures/Texture.h"

	"scenes/Fog.h"
	"scenes/FogExp2.h"
	"scenes/Scene.h"

	"math/Box2.h"
	"math/Box3.h"
	"math/Capsule.h"
	"math/Color.h"
	"math/Cylindrical.h"
	"math/Euler.h"
	"math/float_view.h"
	"math/Frustum.h"
	"math/ImprovedNoise.h"
	"math/Line3.h"
	"math/MathUtils.h"
	"math/Matrix3.h"
	"math/Matrix4.h"
	"math/Plane.h"
	"math/Ray.h"
	"math/Sphere.h"
	"math/Spherical.h"
	"math/SphericalHarmonics3.h"
	"math/Triangle.h"
	"math/Vector2.h"
	"math/Vector3.h"
	"math/Vector4.h"
	"math/Quaternion.h"

	"loaders/ImageLoader.h"

	"utils/StringUtils.h"

)
set(publicHeadersFull)
foreach (header IN LISTS publicHeaders)
    list(APPEND publicHeadersFull "${publicHeaderDir}${header}")
endforeach ()

set(sources 
		"canvas/Canvas.cpp"

		"nodes/core/NodeFrame.cpp"

        "core/BufferGeometry.cpp"
        "core/Clock.cpp"
        "core/EventDispatcher.cpp"
        "core/Layers.cpp"
        "core/Object3D.cpp"
        "core/Raycaster.cpp"
        "core/Uniform.cpp"

		"input/PeripheralsEventSource.cpp"

		"renderers/common/nodes/Nodes.cpp"
		"renderers/common/Info.cpp"
		"renderers/common/Backend.cpp"
		"renderers/common/Renderer.cpp"
		"renderers/common/Attributes.cpp"
		"renderers/common/Geometries.cpp"
		"renderers/common/Animation.cpp"
		"renderers/common/Background.cpp"
		"renderers/common/Textures.cpp"
		"renderers/common/Pipelines.cpp"
		"renderers/common/Bindings.cpp"
		"renderers/common/RenderObject.cpp"
		"renderers/common/RenderObjects.cpp"
		"renderers/common/RenderLists.cpp"
		"renderers/common/RenderContext.cpp"
		"renderers/common/RenderContexts.cpp"

		"renderers/webgpu/utils/WebGPUUtils.cpp"
		"renderers/webgpu/utils/WebGPUAttributeUtils.cpp"
		"renderers/webgpu/utils/WebGPUBindingUtils.cpp"
		"renderers/webgpu/utils/WebGPUPipelineUtils.cpp"
		"renderers/webgpu/utils/WebGPUTextureUtils.cpp"

		"renderers/WebGPUBackend.cpp"
		"renderers/WebGPURenderer.cpp"
		"renderers/RenderTarget.cpp"

		"cameras/Camera.cpp"
        "cameras/PerspectiveCamera.cpp"
        "cameras/OrthographicCamera.cpp"

		"materials/Material.cpp"
		"materials/SpriteMaterial.cpp"

		"objects/Sprite.cpp"

		"lights/AmbientLight.cpp"
        "lights/DirectionalLight.cpp"
        "lights/HemisphereLight.cpp"
        "lights/Light.cpp"
        "lights/LightShadow.cpp"
        "lights/PointLight.cpp"
        "lights/PointLightShadow.cpp"
        "lights/SpotLight.cpp"
        "lights/SpotLightShadow.cpp"

		"scenes/Scene.cpp"
        "scenes/Fog.cpp"
        "scenes/FogExp2.cpp"

		"textures/Texture.cpp"
        "textures/DataTexture3D.cpp"

	    "math/Box2.cpp"
        "math/Box3.cpp"
        "math/Capsule.cpp"
        "math/Color.cpp"
        "math/Cylindrical.cpp"
        "math/Euler.cpp"
        "math/Frustum.cpp"
        "math/ImprovedNoise.cpp"
        "math/Line3.cpp"
        "math/MathUtils.cpp"
        "math/Matrix3.cpp"
        "math/Matrix4.cpp"
        "math/Plane.cpp"
        "math/Ray.cpp"
        "math/Sphere.cpp"
        "math/Spherical.cpp"
        "math/SphericalHarmonics3.cpp"
        "math/Triangle.cpp"
        "math/Vector2.cpp"
        "math/Vector3.cpp"
        "math/Vector4.cpp"
        "math/Quaternion.cpp"

		"loaders/ImageLoader.cpp"

		"utils/StringUtils.cpp"
)

add_library(mix ${sources} ${publicHeadersFull})

target_link_libraries(mix PUBLIC webgpu glfw3webgpu)
