#pragma once

#include <core/Object3D.h>

namespace mix {

    class Bone : public Object3D {

    public:
        [[nodiscard]] std::string type() const override {

            return "Bone";
        }

        static std::shared_ptr<Bone> create() {

            return std::make_shared<Bone>();
        }
    };

}