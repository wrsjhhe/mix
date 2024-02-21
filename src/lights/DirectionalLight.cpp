
#include <lights/DirectionalLight.h>

#include <lights/DirectionalLightShadow.h>


using namespace mix;


DirectionalLight::DirectionalLight(const Color& color, std::optional<float> intensity)
    : Light(color, intensity), LightWithShadow(DirectionalLightShadow::create()) {

    this->position.copy(Object3D::defaultUp);
    this->updateMatrix();
}


std::string DirectionalLight::type() const {

    return "DirectionalLight";
}

void DirectionalLight::dispose() {

    this->shadow->dispose();
}

std::shared_ptr<DirectionalLight> DirectionalLight::create(const Color& color, std::optional<float> intensity) {

    return std::shared_ptr<DirectionalLight>(new DirectionalLight(color, intensity));
}