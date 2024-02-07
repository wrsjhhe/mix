
#include <lights/Light.h>

using namespace mix;


Light::Light(const Color& color, std::optional<float> intensity)
    : color(color), intensity(intensity.value_or(1)) {}


std::string Light::type() const {

    return "Light";
}

void Light::dispose() {}
