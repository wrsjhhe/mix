#pragma once
#include <core/Face3.h>
#include <core/Layers.h>
#include <math/Ray.h>
#include <math/Vector2.h>

#include <limits>
#include <memory>
#include <vector>

namespace mix {

    class Camera;
    class Object3D;

    struct Intersection {

        float distance;
        Vector3 point;
        Object3D* object;

        std::optional<int> index;
        std::optional<int> faceIndex;
        std::optional<Vector2> uv;
        std::optional<Vector2> uv2;
        std::optional<Face3> face;
        std::optional<int> instanceId;
        std::optional<float> distanceToRay;
    };

    class Raycaster {

    public:
        float near;
        float far;

        Ray ray;
        Camera* camera;
        Layers layers;

        struct Params {
            float lineThreshold = 1;
            float pointsThreshold = 1;
        };
        Params params;

        explicit Raycaster(const Vector3& origin = Vector3(), const Vector3& direction = Vector3(), float near = 0, float far = std::numeric_limits<float>::infinity())
            : near(near), far(far), ray(origin, direction), camera(nullptr) {}

        void set(const Vector3& origin, const Vector3& direction);

        void setFromCamera(const Vector2& coords, Camera& camera);

        std::vector<Intersection> intersectObject(Object3D& object, bool recursive = false);

        std::vector<Intersection> intersectObjects(const std::vector<Object3D*>& objects, bool recursive = false);
    };

}
