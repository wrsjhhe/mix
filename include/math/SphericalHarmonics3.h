#pragma once

#include <math/Vector3.h>

#include <vector>

namespace mix {

    class SphericalHarmonis3 {

    public:
        SphericalHarmonis3();

        SphericalHarmonis3& set(const std::vector<Vector3>& coefficients);

        SphericalHarmonis3& zero();

        // get the radiance in the direction of the normal
        // target is a Vector3
        void getAt(const Vector3& normal, Vector3& target);

        // get the irradiance (radiance convolved with cosine lobe) in the direction of the normal
        // target is a Vector3
        // https://graphics.stanford.edu/papers/envmap/envmap.pdf
        void getIrradianceAt(const Vector3& normal, Vector3& target);

        SphericalHarmonis3& add(const SphericalHarmonis3& sh);

        SphericalHarmonis3& addScaledSH(const SphericalHarmonis3& sh, float s);

        SphericalHarmonis3& scale(float s);

        SphericalHarmonis3& lerp(const SphericalHarmonis3& sh, float alpha);

        const std::vector<Vector3>& getCoefficients() const;

    private:
        std::vector<Vector3> coefficients_;
    };

}
