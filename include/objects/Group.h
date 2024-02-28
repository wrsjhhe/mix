#pragma once

#include <core/Object3D.h>

namespace mix {

    class Group : public Object3D {

    public:
        [[nodiscard]] std::string type() const override;

        std::shared_ptr<Object3D> clone(bool recursive = true) override;

        static std::shared_ptr<Group> create();

        ~Group() override = default;
    };

}
