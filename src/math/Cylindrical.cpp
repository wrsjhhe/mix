
#include <math/Cylindrical.h>

#include <math/Vector3.h>

#include <cmath>

using namespace mix;

Cylindrical::Cylindrical(float radius, float theta, float y): radius(radius), theta(theta), y(y) {}

Cylindrical& Cylindrical::set(float radius, float theta, float y) {

    this->radius = radius;
    this->theta = theta;
    this->y = y;

    return *this;
}

Cylindrical& Cylindrical::copy(const Cylindrical& other) {

    this->radius = other.radius;
    this->theta = other.theta;
    this->y = other.y;

    return *this;
}

Cylindrical& Cylindrical::setFromVector3(const Vector3& v) {

    return this->setFromCartesianCoords(v.x, v.y, v.z);
}

Cylindrical& Cylindrical::setFromCartesianCoords(float x, float y, float z) {

    this->radius = std::sqrt(x * x + z * z);
    this->theta = std::atan2(x, z);
    this->y = y;

    return *this;
}
