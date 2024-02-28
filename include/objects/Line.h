#pragma once

#include <core/BufferGeometry.h>
#include <core/Object3D.h>
#include <materials/Material.h>

#include <memory>
#include <utility>
#include <vector>

namespace mix {

    class Line : public Object3D {

    public:
        Line(std::shared_ptr<BufferGeometry> geometry, std::shared_ptr<Material> material);

        [[nodiscard]] std::string type() const override;

        BufferGeometry* geometry() override;

        Material* material() override;

        std::vector<Material*> materials() override;

        virtual void computeLineDistances();

        void raycast(Raycaster& raycaster, std::vector<Intersection>& intersects) override;

        std::shared_ptr<Object3D> clone(bool recursive = true) override;

        static std::shared_ptr<Line> create(const std::shared_ptr<BufferGeometry>& geometry = nullptr, const std::shared_ptr<Material>& material = nullptr);

    protected:
        std::shared_ptr<BufferGeometry> geometry_;
        std::shared_ptr<Material> material_;
    };

}