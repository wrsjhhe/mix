#include <materials/ShaderMaterial.h>


using namespace mix;

ShaderMaterial::ShaderMaterial()
    : MaterialWithLights(false),
    MaterialWithClipping(false),
    MaterialWithWireframe(false, 1),
    MaterialWithLineWidth(1),
    uniforms(std::make_shared<UniformMap>()) {

    this->fog = false;
    this->lights = false;
    this->clipping = false;

    defaultAttributeValues["color"] = Color(1, 1, 1);
    defaultAttributeValues["uv"] = Vector2(0, 0);
    defaultAttributeValues["uv2"] = Vector2(0, 0);
}


std::string ShaderMaterial::type() const {

    return "ShaderMaterial";
}

std::shared_ptr<ShaderMaterial> ShaderMaterial::create() {

    return std::shared_ptr<ShaderMaterial>(new ShaderMaterial());
}