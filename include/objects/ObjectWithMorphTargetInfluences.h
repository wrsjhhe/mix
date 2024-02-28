#pragma once

#include <vector>

namespace mix {

    struct ObjectWithMorphTargetInfluences {

        std::vector<float>& morphTargetInfluences() {

            if (copyMorphTargetInfluences_) {
                return *copyMorphTargetInfluences_;
            }

            return morphTargetInfluences_;
        }

        void copyMorphTargetInfluences(std::vector<float>* influences) {

            copyMorphTargetInfluences_ = influences;
        }

        void reset() {

            copyMorphTargetInfluences_ = nullptr;
        }

    private:
        std::vector<float> morphTargetInfluences_;
        std::vector<float>* copyMorphTargetInfluences_ = nullptr;
    };

}
