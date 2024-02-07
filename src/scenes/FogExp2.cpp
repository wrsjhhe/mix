
#include <scenes/FogExp2.h>

using namespace mix;


FogExp2::FogExp2(const Color& hex, float density): color(hex), density(density) {}

bool FogExp2::operator==(const FogExp2& f) const {

    return f.color == this->color && f.density == this->density;
}
