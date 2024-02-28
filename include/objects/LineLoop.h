#pragma once

#include <objects/Line.h>

namespace mix {

    class LineLoop : public Line {

    public:
        LineLoop(const std::shared_ptr<BufferGeometry>& geometry, const std::shared_ptr<Material>& material);

        [[nodiscard]] std::string type() const override;

        std::shared_ptr<Object3D> clone(bool recursive = true) override;

        static std::shared_ptr<LineLoop> create(
            const std::shared_ptr<BufferGeometry>& geometry = nullptr,
            const std::shared_ptr<Material>& material = nullptr);
    };

}
