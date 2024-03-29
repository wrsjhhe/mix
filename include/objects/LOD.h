#pragma once

#include <core/Object3D.h>

#include <utility>

namespace mix {

    class Camera;

    struct Level {

        float distance;
        Object3D* object;

        Level(float distance, Object3D* object)
            : distance(distance), object(object) {}
    };

    class LOD : public Object3D {

    public:
        bool autoUpdate = true;

        LOD() = default;

        [[nodiscard]] std::string type() const override;

        LOD& addLevel(Object3D& object, float distance = 0);

        LOD& addLevel(const std::shared_ptr<Object3D>& object, float distance = 0);

        [[nodiscard]] size_t getCurrentLevel() const;

        void update(Camera& camera);

        static std::shared_ptr<LOD> create();

    private:
        size_t _currentLevel = 0;
        std::vector<Level> levels;
    };

}
