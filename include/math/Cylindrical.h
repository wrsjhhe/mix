#pragma once

namespace mix {

    class Vector3;

    class Cylindrical {

    public:
        float radius;
        float theta;
        float y;

        explicit Cylindrical(float radius = 1, float theta = 0, float y = 0);

        Cylindrical& set(float radius, float theta, float y);

        Cylindrical& copy(const Cylindrical& other);

        Cylindrical& setFromVector3(const Vector3& v);

        Cylindrical& setFromCartesianCoords(float x, float y, float z);
    };

}
