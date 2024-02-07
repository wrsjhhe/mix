
#include <lights/PointLight.h>

#include <cameras/PerspectiveCamera.h>
#include <math/MathUtils.h>

#include <lights/PointLightShadow.h>

using namespace mix;


PointLight::PointLight(const Color& color, std::optional<float> intensity, float distance, float decay)
    : Light(color, intensity), LightWithShadow(PointLightShadow::create()), distance(distance), decay(decay) {}


std::string PointLight::type() const {

    return "PointLight";
}

float PointLight::getPower() const {

    // intensity = power per solid angle.
    // ref: equation (15) from https://seblagarde.files.wordpress.com/2015/07/course_notes_moving_frostbite_to_pbr_v32.pdf
    return this->intensity * 4.f * math::PI;
}

void PointLight::setPower(float power) {

    // intensity = power per solid angle.
    // ref: equation (15) from https://seblagarde.files.wordpress.com/2015/07/course_notes_moving_frostbite_to_pbr_v32.pdf
    this->intensity = power / (4.f * math::PI);
}

void PointLight::dispose() {

    this->shadow->dispose();
}

std::shared_ptr<PointLight> PointLight::create(const Color& color, std::optional<float> intensity, float distance, float decay) {

    return std::shared_ptr<PointLight>(new PointLight(color, intensity, distance, decay));
}
